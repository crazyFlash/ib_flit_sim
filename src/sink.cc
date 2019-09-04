///////////////////////////////////////////////////////////////////////////
//
//         InfiniBand FLIT (Credit) Level OMNet++ Simulation Model
//
// Copyright (c) 2004-2013 Mellanox Technologies, Ltd. All rights reserved.
// This software is available to you under the terms of the GNU
// General Public License (GPL) Version 2, available from the file
// COPYING in the main directory of this source tree.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////
//
// The IBSink implements an IB endport FIFO that is filled by the push 
// message and drained by the pop self-message.
// To simulate hiccups in PCI Exp bus, we schedule self-messages of type hiccup
// hiccup message alternate between an ON and OFF state. During ON state any
// drain message is ignored. On transition to OFF a new drain message can
// be generated
// 
#include "ib_m.h"
#include "sink.h"

Define_Module( IBSink );

void IBSink::initialize()
{
  waitStats.setName("Waiting time statistics");
  hiccupStats.setName("Hiccup Statistics");
  maxVL = par("maxVL");
  startStatCol_sec = par("startStatCol");
  lid = getParentModule()->par("srcLid");
  PakcetFabricTime.setName("Packet Fabric Time");
//  PakcetFabricTime.setRangeAutoUpper(0, 10, 1.5);
  PakcetFabricTime.setRange(0, NAN);
  PakcetFabricTime.setNumPrecollectedValues(10);
  PakcetFabricTime.setRangeExtensionFactor(1.5);


  // calculate the drain rate
  // 流失率 计算
  flitSize = par("flitSize");
  popDlyPerByte_ns = par("popDlyPerByte"); // PCIe drain rate   //PCIE发送时延
  WATCH(popDlyPerByte_ns);
 
  repFirstPackets = par("repFirstPackets");

  // we will allocate a drain message only on the first flit getting in
  // which is consumed immediately...
  p_drainMsg = new cMessage("pop", IB_POP_MSG); // POP类型
  AccBytesRcv = 0;
  
  duringHiccup = 0;
  WATCH(duringHiccup);
  
  p_hiccupMsg = new cMessage("pop");
  p_hiccupMsg->setKind(IB_HICCUP_MSG);
  // 定时向自己发送HICCUP 消息 ####################################################################################
  scheduleAt(simTime()+1e-9, p_hiccupMsg);
  
  // we track number of packets per VL:
  for (int vl = 0; vl < maxVL+1; vl++) 
    VlFlits.push_back(0);
  
  WATCH_VECTOR(VlFlits);

  totOOOPackets = 0;
  totIOPackets = 0;
  totOOPackets = 0;
  // OOO IO OO package, 不同类型的包
  oooPackets.setName("OOO-Packets");
  oooWindow.setName("OOO-Window-Pkts");
  msgLatency.setName("Msg-Network-Latency");
  msgF2FLatency.setName("Msg-First2First-Network-Latency");
  enoughPktsLatency.setName("Enough-Pkts-Network-Latency");
  enoughToLastPktLatencyStat.setName("Last-to-Enough-Pkt-Arrival");
}

// Init a new drain message and schedule it after delay
/**
 * 向自己发送一个drain message，理解为一个即失性消息
 * 作用且待后看
 */
void IBSink::newDrainMessage(double delay_us) {
  // we track the start time so we can hiccup left over...
  p_drainMsg->setTimestamp(simTime()); 
  // 定时发送一个即失性消息 ##################################################################################
  scheduleAt(simTime()+delay_us*1e-6, p_drainMsg);
}

// track consumed messages and send "sent" event to the IBUF
// 向IBUF 发送一个Send通知信息
void IBSink::consumeDataMsg(IBDataMsg *p_msg)
{

  EV << "-I- " << getFullPath() << " consumed data:" 
     << p_msg->getName() << endl;

  // track the absolute time this packet was consumed
  lastConsumedPakcet = simTime();

  // track the time this flit waited in the HCA
  // 计算等待HCA等待时间
  if (simTime() > startStatCol_sec) {
	 simtime_t d = lastConsumedPakcet - p_msg->getTimestamp();
	 waitStats.collect( d );
	 
	 // track the time this flit spent on the wire...
	 if (p_msg->getFlitSn() == (p_msg->getPacketLength() -1)) {
		d = simTime() - p_msg->getTimestamp();
		PakcetFabricTime.collect( d );
	 }
  }

  int vl = p_msg->getVL();
  VlFlits[vl]++;

  IBSentMsg *p_sentMsg = new IBSentMsg("hca_sent", IB_SENT_MSG);
  p_sentMsg->setVL(vl);
  p_sentMsg->setWasLast(p_msg->getPacketLength() == p_msg->getFlitSn() + 1);

  // 向sent口发送一个send 通知信息，sent 口 与IBUF 相连 #############################################################
  send(p_sentMsg, "sent");
  delete p_msg;
}

/**
 * 处理数据型消息
 */
void IBSink::handleData(IBDataMsg *p_msg)
{
  double delay_us;

  // make sure was correctly received (no routing bug)
  // 确保路由无误，是自己的数据包
  if (p_msg->getDstLid() != (int)lid) {
      throw cRuntimeError("-E- Received packet to %d while self lid is %d",
			  p_msg->getDstLid() , lid);
  }

  // for head of packet calculate out of order
  if (p_msg->getFlitSn() == 0) {
	  unsigned int srcLid = p_msg->getSrcLid();
	  unsigned int srcPktSn = p_msg->getPacketSn(); //当前消息包序号

	  //查找当前源source id是否已被记录
	 if (lastPktSnPerSrc.find(srcLid) != lastPktSnPerSrc.end()) {
		  unsigned int curSn = lastPktSnPerSrc[srcLid]; //srcLid源下的上一个成功接收的包序号
		  if (srcPktSn == 1+curSn) {    // 当前序号 = 上一个序号+1
			  // OK case
			  lastPktSnPerSrc[srcLid]++;    //序号+
			  totIOPackets++;
		  } else if (srcPktSn < curSn) {
			  // We do not count tail as OOO
		  } else if (srcPktSn > 1+curSn) {      // OOO啥意思啊，这个序号大于接收的上一个序号+1，说明中间序号没有被接收到
			  // OOO was received
			  totOOOPackets++;
			  totOOPackets += srcPktSn - curSn;
			  oooPackets.record(totOOOPackets);
			  lastPktSnPerSrc[srcLid] = srcPktSn;   // 直接跳过？中间的序号没加起来怎么算
			  oooWindow.collect(srcPktSn-curSn);
		  } else if (srcPktSn == curSn) {
			  // this is a BUG!
		      throw cRuntimeError("-E- Received packet to %d from %d with PacketSn %d equal to previous Sn",
					  p_msg->getDstLid() , srcLid, srcPktSn);
		  } else {
			  // Could not get here - A bug
		      throw cRuntimeError("BUG: IBSink::handleData unexpected relation of curSn %d and PacketSn %d",
					  curSn, srcPktSn);
		 }
	 } else {
	     //没有则直接新纪录一个
		 lastPktSnPerSrc[srcLid] = srcPktSn;    // 这也直接记录了？？？
		 totIOPackets++;
	 }
  }

  // calculate message latency - we track the "first" N packets of the message
  // we clean only all of them are received
  std::map<MsgTupple, class OutstandingMsgData, MsgTuppleLess>::iterator mI;

  // for first flits
  if (p_msg->getFlitSn() == 0) {
	  MsgTupple mt(p_msg->getSrcLid(), p_msg->getAppIdx(), p_msg->getMsgIdx());
	  mI = outstandingMsgsData.find(mt);
	  if (mI == outstandingMsgsData.end()) {
		  EV << "-I- " << getFullPath() << " received first flit of new message from src: "
			 <<  p_msg->getSrcLid() << " app:" << p_msg->getAppIdx() << " msg: " << p_msg->getMsgIdx() << endl;
		  outstandingMsgsData[mt].firstFlitTime = p_msg->getInjectionTime();
	  }

	  // first flit of the last packet
	  if (outstandingMsgsData[mt].numPktsReceived + 1 == (unsigned int)p_msg->getMsgLen()) {
	    double f2fLat = simTime().dbl() -  outstandingMsgsData[mt].firstFlitTime.dbl();
	    msgF2FLatency.collect(f2fLat);
	  }
  }

  // can not use else here as we want to handle single flit packets
  if (p_msg->getFlitSn() == p_msg->getPacketLength() - 1) {
	  // last flit of a packet
	  MsgTupple mt(p_msg->getSrcLid(), p_msg->getAppIdx(), p_msg->getMsgIdx());
	  mI = outstandingMsgsData.find(mt);
	  if (mI == outstandingMsgsData.end()) {
	      throw cRuntimeError("-E- Received last flit of packet from %d with no corresponding message record", p_msg->getSrcLid());
	  }
	  (*mI).second.numPktsReceived++;
	  EV << "-I- " << getFullPath() << " received last flit of packet: " << (*mI).second.numPktsReceived << " from src: "
	  <<  p_msg->getSrcLid() << " app:" << p_msg->getAppIdx() << " msg: " << p_msg->getMsgIdx() << endl;

	  // track the latency of the first num pkts of message
	  if (repFirstPackets) {
		  if ( (*mI).second.numPktsReceived == repFirstPackets) {
			  EV << "-I- " << getFullPath() << " received enough (" << repFirstPackets << ") packets for message from src: "
					 <<  p_msg->getSrcLid() << " app:" << p_msg->getAppIdx() << " msg: " << p_msg->getMsgIdx() << endl;
			  enoughPktsLatency.collect(simTime() - (*mI).second.firstFlitTime);
			  (*mI).second.enoughPktsLastFlitTime = simTime();
		  }
	  }

	  // clean completed messages
	  if ((*mI).second.numPktsReceived == (unsigned int)p_msg->getMsgLen()) {
		  if (repFirstPackets) {
			  enoughToLastPktLatencyStat.collect(simTime() - (*mI).second.enoughPktsLastFlitTime);
		  }
		  msgLatency.collect(simTime() - (*mI).second.firstFlitTime);
		  EV << "-I- " << getFullPath() << " received last flit of message from src: "
				 <<  p_msg->getSrcLid() << " app:" << p_msg->getAppIdx() << " msg: " << p_msg->getMsgIdx() << endl;
		  outstandingMsgsData.erase(mt);
	  }
  }

  // for iBW calculations
  if (simTime() >= startStatCol_sec) {
	 AccBytesRcv += p_msg->getByteLength(); // p_msg->getBitLength()/8;
  }

  // we might be arriving on empty buffer:
  if ( ! p_drainMsg->isScheduled() ) {
    EV << "-I- " << getFullPath() << " data:" << p_msg->getName() 
       << " arrived on empty FIFO" << endl;
    // this credit should take this time consume:
    delay_us = p_msg->getByteLength() * popDlyPerByte_ns*1e-3;
    newDrainMessage(delay_us);
  }

  EV << "-I- " << getFullPath() << " queued data:" << p_msg->getName() << endl;
  queue.insert(p_msg);
}

// simply consume one message from the Q or stop the drain if Q is empty
// also under hiccup do nothing
// drain message和POP的关系是如何？？？
void IBSink::handlePop(cMessage *p_msg)
{
  // if we are under hiccup - do nothing or
  // got to pop from the queue if anything there
  if ( !queue.isEmpty() && ! duringHiccup ) {
    IBDataMsg *p_dataMsg = (IBDataMsg *)queue.pop();
    EV << "-I- " << getFullPath() << " De-queued data:" 
       << p_dataMsg->getName() << endl;

    // when is our next pop event?
    double delay_ns = p_dataMsg->getByteLength() * popDlyPerByte_ns;
    
    // consume actually discards the message !!!
    consumeDataMsg(p_dataMsg);          //从队列中pop出一个消息，然后删除
    
    scheduleAt(simTime()+delay_ns*1e-9, p_drainMsg);
  } else {
    // The queue is empty. Next message needs to immediatly pop
    // so we clean the drain event
    EV << "-I- " << getFullPath() << " Nothing to POP" << endl;
    cancelEvent(p_drainMsg);
  }
}

// hickup really means we  drain and set another one.
// Hiccup是什么阶段？
void IBSink::handleHiccup(cMessage *p_msg)
{
  simtime_t delay_us;

  if ( duringHiccup ) {
    // we are inside a hiccup - turn it off and schedule next ON
    duringHiccup = 0;
    delay_us = par("hiccupDelay");
    EV << "-I- " << getFullPath() << " Hiccup OFF for:" 
       << delay_us << "usec" << endl;

    // as we are out of hiccup make sure we have at least one outstanding drain
    if (! p_drainMsg->isScheduled())
      newDrainMessage(1e-3); // 1ns     为什么是1ns，这个是怎么确定的？？
  } else {
    // we need to start a new hiccup
    duringHiccup = 1;
    delay_us = par("hiccupDuration");
    
    EV << "-I- " << getFullPath() << " Hiccup ON for:" << delay_us 
       << "usec" << endl ;
  }

  hiccupStats.collect( simTime() );
  scheduleAt(simTime()+delay_us*1e-6, p_hiccupMsg);
}

/**
 * 不同类型消息处理
 */
void IBSink::handleMessage(cMessage *p_msg)
{
  simtime_t delay;
  int kind = p_msg->getKind();

  if ( kind == IB_DATA_MSG ) {
    handleData((IBDataMsg *)p_msg);
  } else if ( kind == IB_POP_MSG ) {
    handlePop(p_msg);
  } else if ( kind == IB_HICCUP_MSG ) {
    handleHiccup(p_msg);
  } else if ( kind == IB_FLOWCTRL_MSG ) {
      //流控制类型
    EV << "-I- " << getFullPath() << " Dropping flow control message";
    delete p_msg;
  } else if ( kind == IB_DONE_MSG ) {
    delete p_msg;
  } else {
      throw cRuntimeError("-E- %s does not know what to with msg: %d is local: %d"
              " senderModule: %s", 
              getFullPath().c_str(), 
              p_msg->getKind(), 
              p_msg->isSelfMessage(), 
              p_msg->getSenderModule());
    delete p_msg;
  }
}

// recordScalar 记录到哪里去了
void IBSink::finish()
{
  char buf[128];
  recordScalar("Time last packet consumed:", lastConsumedPakcet);
  waitStats.record();
  PakcetFabricTime.record();
  msgLatency.record();
  msgF2FLatency.record();
  enoughPktsLatency.record();
  enoughToLastPktLatencyStat.record();

  double iBW = AccBytesRcv / (simTime() - startStatCol_sec);
  recordScalar("Sink-BW-MBps", iBW/1e6);
  for (int vl = 0; vl < maxVL+1; vl++) {
    sprintf(buf, "VL-%d-total-flits", vl);
    recordScalar(buf, VlFlits[vl]);
  }
  oooWindow.record();
  recordScalar("OO-IO-Packets-Ratio", 1.0*totOOPackets/totIOPackets);
  recordScalar("Num-SRCs", lastPktSnPerSrc.size());
  lastPktSnPerSrc.clear();
}

IBSink::~IBSink() {
	if (p_drainMsg)
		cancelAndDelete(p_drainMsg);
}

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
// IB FLITs Generator.
// Send IB FLITs one at a time. Received Messages from a set of Applications
// 
// Internal Messages: 
// push - inject a new data FLIT into the Queues in PCIe speed and jitter
//
// External Messages:
// sent - IN: a vHoQ was sent by the VLA (carry the VL)
// msgDone - OUT: tell the App that the message was sent
// msg - IN: receive a message from the App
//
// For a full description read the gen.h
//

#include "ib_m.h"
#include "gen.h"
#include "vlarb.h"
#include "vec_file.h"

Define_Module(IBGenerator);

// main init of the module
void IBGenerator::initialize() {
  // General non-volatile parameters
  srcLid = par("srcLid");
  flitSize_B = par("flitSize"); 
  genDlyPerByte_ns = par("genDlyPerByte");  //字节时延，有待修改

  // statistics
  timeLastSent = 0;
  totalBytesSent = 0;
  firstPktSendTime = 0;
  
  // we start with pkt 0
  pktId = 0;
  msgIdx = 0;

  // init the vector of incoming messages
  numApps = gateSize("in");     //number of app , in门的数量？？
  appMsgs.resize(numApps, NULL);
  curApp = 0;
  numContPkts = 0;
  maxContPkts = par("maxContPkts");
  maxQueuedPerVL = par("maxQueuedPerVL");

  // push meassage, 干啥的啊，push1
  pushMsg = new cMessage("push1", IB_PUSH_MSG);     // 这里产生push类型的消息，后面祥看
  // APP 产生消息开始
  // no need for self start
}

// initialize the packet with index pktIdx parameters on the message
// 设置package信息，包长，包数量，Flit数量，flit字节长
void IBGenerator::initPacketParams(IBAppMsg *p_msg, unsigned int pktIdx)
{
  // double check
  // package index 大于消息的包数量
  if (pktIdx >= p_msg->getLenPkts()) {
    error("try initPacketParams with index %d > lenPkts %d",
          pktIdx, p_msg->getLenPkts());
  }

  // zero the FLIT index
  p_msg->setFlitIdx(0);
  p_msg->setPktIdx(pktIdx);
  p_msg->setVL(vlBySQ(p_msg->getSQ()));

  unsigned int pktLen_B;    //包大小，MTU或者小于MTU
  unsigned int pktLen_F;    //不懂这个参数！！！！！！！！！！！！！！！！！！！！！！
  std::cout <<"msgIndex: " <<p_msg->getPktIdx() <<endl;

  // length of last msg packet may be smaller
  if (p_msg->getPktIdx() >= p_msg->getLenPkts()- 1) { // index begin from 0
    // last packet
    //最后一个包，长度为消息总长度对MTU取模
    pktLen_B = p_msg->getLenBytes() % p_msg->getMtuBytes();
    std::cout <<"-------------------->line 97. pktLen_B: " <<pktLen_B <<endl;
    if (pktLen_B == 0) 
      pktLen_B = p_msg->getMtuBytes();
  } else {
      //不是最后一个包，则每次都是一个MTU的长度
    pktLen_B = p_msg->getMtuBytes();
    std::cout <<"-------------------->line 103. pktLen_B: " <<pktLen_B <<endl;
  }
  pktLen_F = (pktLen_B + flitSize_B - 1) / flitSize_B;      //在分片MTU的基础上再次细分了，分为几个FlitSzie
  p_msg->setPktLenBytes(pktLen_F*flitSize_B);
  p_msg->setPktLenFlits(pktLen_F);
}

// find the VLA and check it HoQ is free...
// NOTE THIS WILL LOCK THE HoQ - MUST IMMEDIATLY PLACE THE FLIT THERE
//
int IBGenerator::isRemoteHoQFree(int vl){
  // find the VLA connected to the given port and
  // call its method for checking and setting HoQ
  cGate *p_gate = gate("out")->getPathEndGate();
  IBVLArb *p_vla = dynamic_cast<IBVLArb *>(p_gate->getOwnerModule());
  if ((p_vla == NULL) || strcmp(p_vla->getName(), "vlarb")) {
    error("cannot get VLA for generator out port");
  }
  
  int remotePortNum = p_gate->getIndex();
  return(p_vla->isHoQFree(remotePortNum, vl));
}

// emmmm这个函数
unsigned int IBGenerator::vlBySQ(unsigned sq) 
{
  return(sq);
}

// scan through the available applications and schedule next one
// take current VLQ threshold and maxContPkts into account
// updates curApp
// return true if found new appMsg to work on
//#################################################################################
bool IBGenerator::arbitrateApps() 
{
  // try to stay with current app if possible
  if (appMsgs[curApp]) {
    unsigned vl = vlBySQ(appMsgs[curApp]->getSQ());
    if ((numContPkts < maxContPkts) && 
        ((unsigned)VLQ[vl].getLength() < maxQueuedPerVL)) {
      EV << "-I-" << getFullPath() << " arbitrate apps continue" << endl;
      return true;
    }
  }

  unsigned int oldApp = curApp;
  bool found = false;
  // search through all apps return to current 
  for (unsigned i = 1; !found && (i <= numApps); i++) {
    unsigned int a = (curApp + i) % numApps;
    EV << "-I-" << getFullPath() << " trying app: " << a << endl;
    if (appMsgs[a]) {
      unsigned vl = vlBySQ(appMsgs[a]->getSQ());
      if ((unsigned)VLQ[vl].getLength() < maxQueuedPerVL) {
        curApp = a;
        EV << "-I-" << getFullPath() << " arbitrate apps selected:" 
           << a << endl;
        found = true;
      } else {
        EV << "-I-" << getFullPath() << " skipping app:" << a
           << " since VLQ[" << vl << "] is full" << endl;
      }
    }
  }

  if (oldApp != curApp) {
    numContPkts = 0;
  } else {
    numContPkts++;
  }

  if (!found) {
      EV << "-I-" << getFullPath() << " arbitrate apps found no app" << endl;
  }
  return found;
}

// Called when there is some active appMsg that can be
// handled. Create the FLIT and place on VLQ, Maybe send (if VLA empty)
// also may retire the appMsg and clean the appMsgs and send it back to 
// its app
void IBGenerator::getNextAppMsg()
{
  IBAppMsg *p_msg = appMsgs[curApp];

  // IN THE MSG CONECT WE ALWAYS STORE NEXT (TO BE SENT) FLIT AND PKT INDEX

  // incremeant flit idx:
  unsigned int thisFlitIdx = p_msg->getFlitIdx();
  unsigned int thisPktIdx = p_msg->getPktIdx();
  unsigned int thisMsgIdx = p_msg->getMsgIdx();
  unsigned int thisMsgLen = p_msg->getLenPkts();    //包数量
  unsigned int thisAppIdx = p_msg->getAppIdx();
  unsigned int thisPktDst = p_msg->getDstLid();

  // now make the new FLIT:
  // ################################# 这里做了另一层消息的封装，类似TCP/IP不同层的消息封装
  IBDataMsg *p_cred;
  char name[128];
  sprintf(name, "data-%d-%d-%d-%d", srcLid, msgIdx, thisPktIdx, thisFlitIdx);
  p_cred = new IBDataMsg(name, IB_DATA_MSG);

  p_cred->setSrcLid(srcLid);
  p_cred->setBitLength(flitSize_B*8);
  p_cred->setByteLength(flitSize_B);    //单条消息长度就是FlitSize

  p_cred->setDstLid(thisPktDst);
  p_cred->setSL(p_msg->getSQ());
  p_cred->setVL(p_msg->getVL());

  p_cred->setFlitSn(thisFlitIdx);
  p_cred->setPacketId(thisPktIdx);
  p_cred->setMsgIdx(thisMsgIdx);
  p_cred->setAppIdx(thisAppIdx);
  p_cred->setPktIdx(thisPktIdx);
  p_cred->setMsgLen(thisMsgLen);    //消息长度用包数量表示
  p_cred->setPacketLength(p_msg->getPktLenFlits()); //包的flit数量
  p_cred->setPacketLengthBytes(p_msg->getPktLenBytes());    //获取包的字节长度，flit数量*flitSize

  p_cred->setBeforeAnySwitch(true);

  // provide serial number to packet head flits
  if (thisFlitIdx == 0) {
	  unsigned int dstPktSn = 0;
	  if (lastPktSnPerDst.find(thisPktDst) == lastPktSnPerDst.end()) {
		  dstPktSn = 1;
		  lastPktSnPerDst[thisPktDst] = dstPktSn;
	  } else {
		  dstPktSn = ++lastPktSnPerDst[thisPktDst];
	  }
	  p_cred->setPacketSn(dstPktSn);
  } else {
	  p_cred->setPacketSn(0);
  }

  // now we have a new FLIT at hand we can either Q it or send it over 
  // if there is a place for it in the VLA 
  // 查看接口是否空闲可发送，如果空闲，则直接发送
  unsigned int vl = p_msg->getVL();
  if (VLQ[vl].isEmpty() && isRemoteHoQFree(vl)) {
    sendDataOut(p_cred);
  } else {
      //否则，将该消息入队列
    VLQ[vl].insert(p_cred);
    EV << "-I- " << getFullPath() << " Queue new FLIT " << p_cred->getName() << " as HoQ not free for vl:"
       << vl << endl;
  }
  // 其实没必要分情况，总是都是要有一个入队的操作，然后从队列中发送一个

  // now anvance to next FLIT or declare the app msg done

  // decide if we are at end of packet or not
  // 同样判断是否是最后一个包
  if (++thisFlitIdx == p_msg->getPktLenFlits()) {
    // we completed a packet was it the last?
    if (++thisPktIdx == p_msg->getLenPkts()) {
      // we are done with the app msg
      EV << "-I- " << getFullPath() << " completed appMsg:" 
         << p_msg->getName() << endl;

      // 发送APP消息，in门。。。这是从APP发送出去，还是发送回来APP啊？？？
      // 因为in是对接 APP 的，所以这个是发送回来给APP的消息，对外为out
      std::cout <<"currtime: " <<simTime() <<endl;
      finish();
//      send(p_msg, "in$o", curApp);


      appMsgs[curApp] = NULL;
    } else {
      p_msg->setPktIdx(thisPktIdx);
      initPacketParams(p_msg, thisPktIdx);
    }
  } else {
    p_msg->setFlitIdx(thisFlitIdx);
  }
}

// arbitrate for next app, generate its FLIT and schedule next push
// 看起来啥事儿没敢，
void IBGenerator::genNextAppFLIT() 
{
  // get the next application to work on
  if (!arbitrateApps()) {
    // may be we do not have anything to do
    if (pushMsg->isScheduled()) {
      cancelEvent(pushMsg);
    }
    return;
  }

  // place the next app msg FLIT into the VLQ and maybe send it
  getNextAppMsg(); // 获取下一个发送APP的消息，也即发送或者存入队列

  // schedule next push
  // push self消息是要干嘛？这个也没看懂
  // 当即发送一条push消息，执行genNextAppFLIT, gen的时间应该和pop的时间相同，传输完成之后立刻进行下一个FLIT的传输
  simtime_t delay = genDlyPerByte_ns*1e-9*flitSize_B;
  scheduleAt(simTime()+delay, pushMsg);
}

// push is an internal event for generating a new FLIT
// push 类型消息处理
void IBGenerator::handlePush(cMessage *p_msg){
  // arbitrate next app, 
  genNextAppFLIT();
}

// when a new application message in provided
// APP message消息处理
void IBGenerator::handleApp(IBAppMsg *p_msg){
  // decide what port it was provided on
  unsigned int a = p_msg->getArrivalGate()->getIndex();

//  ##########################################################################################################################
  // VL初始为0
//  std::cout <<"length: " <<p_msg->getLenBytes() <<"\t.MTU: " <<p_msg->getMtuBytes()
//          <<"\t.packetLength: " <<p_msg->getLenPkts() <<"\t. VL: " <<p_msg->getVL() <<endl;

  // check that the app is empty or error
  if (appMsgs[a] != NULL) {
    error("provided app %d message but app not empty!", a);
  }

  // count total of messages injected
  msgIdx++;

  // init the first packet parameters
  initPacketParams(p_msg, 0);
  
  // store it
  appMsgs[a] = p_msg;
  
  // if there is curApp msg or waiting on push pushMsg = do nothing
  if (((curApp != a) && (appMsgs[curApp] != NULL)) || ( pushMsg->isScheduled())) {
    EV << "-I-" << getFullPath() << " new app message:" << p_msg->getName()
       << " queued since previous message:" << appMsgs[curApp]->getName() 
       << " being served" << endl;
    return;
  }

  // force the new app to be arbitrated
  curApp = a;

  genNextAppFLIT();
}

// send out data and wait for it to clear
// 发送对外数据消息
void IBGenerator::sendDataOut(IBDataMsg *p_msg){
  unsigned int bytes = p_msg->getByteLength();
  double delay_ns = ((double)par("popDlyPerByte"))*bytes;

  std::cout <<"---------------------Bytes: " <<bytes <<". deLays: " <<delay_ns
          <<". popDlyPerByte: " <<(double)par("popDlyPerByte") <<". flitId: " <<p_msg->getFlitSn() <<"---------------" <<endl;

  // 计算发送时延
  // time stamp to enable tracking time in Fabric
  p_msg->setInjectionTime(simTime()+delay_ns*1e-9);
  p_msg->setTimestamp(simTime()+delay_ns*1e-9);
  totalBytesSent += bytes;

//  if(totalBytesSent >= (unsigned int)par("msgLength")){
//      std::cout <<"toatlBytesSent: " <<totalBytesSent <<endl;
//  }

  sendDelayed(p_msg, delay_ns*1e-9, "out");

  EV << "-I- " << getFullPath() 
     << " sending " << p_msg->getName() 
     << " packetLength(B):" << bytes
     << " flitSn:" << p_msg->getFlitSn() 
     << " dstLid:" << p_msg->getDstLid() 
     << endl;
  
  // For oBW calculations
  if (firstPktSendTime == 0) 
    firstPktSendTime = simTime();   // 记录第一个包发出时的仿真时间
}

// when the VLA has sent a message
// sent 消息表示一个通知。告诉 gen 需要对外发送一条data消息
void IBGenerator::handleSent(IBSentMsg *p_sent){
   // IBSentMsg 只是个通知信息，提醒是第几条了，然后从队列中取消息
  int vl = p_sent->getVL();
  // We can not just send - need to see if the HoQ is free...
  // NOTE : since we LOCK the HoQ when asking if HoQ is free we 
  // must make sure we have something to send before we ask about it

  std::cout <<"---------------get sent back! time: " <<simTime() <<endl;
  // 当前序号的消息判断队列是否为空，为空则表示已经发送，这条Send信息没啥用
  // 如果是非空，则需要执行发送
  if (!VLQ[vl].isEmpty()) {
    if (isRemoteHoQFree(vl)) {
      IBDataMsg *p_msg = (IBDataMsg *)VLQ[vl].pop();

      // IBDataMsg 才是要发送的消息，由SendDataOut函数执行发送
      EV << "-I- " << getFullPath() << " de-queue packet:"
         << p_msg->getName()<< " at time " << simTime() << endl;
      sendDataOut(p_msg);

      // since we popped a message we may have now free'd some space
      // if there is no shceduled push ...
      if (!pushMsg->isScheduled()) {
        simtime_t delay = genDlyPerByte_ns*1e-9*flitSize_B;
        scheduleAt(simTime()+delay, pushMsg);
      }
    } else {
      EV << "-I- " << getFullPath() << " HoQ not free for vl:" << vl << endl;
    }
  } else {
    EV << "-I- " << getFullPath() << " nothing to send on vl:" << vl << endl;
  }
  delete p_sent;
}

// 消息处理，选择不同类型的消息处理。这里其实可以做一个多态处理，因为继承自同一个父类，函数调用直接传入子类即可；
// 个人感觉不需要先封装成父类cMessage类型，添加一个类型属性，然后再分，没必要！！！
void IBGenerator::handleMessage(cMessage *p_msg) {
  int msgType = p_msg->getKind();
  if ( msgType == IB_SENT_MSG ) {
    handleSent((IBSentMsg *)p_msg);
  } else if ( msgType == IB_APP_MSG ) {
    handleApp((IBAppMsg*)p_msg);
  } else {
    handlePush(p_msg);
  }
}

void IBGenerator::finish()
{
  double oBW = totalBytesSent / (simTime() - firstPktSendTime);
  std::cout <<"totalByteSent: " <<totalBytesSent <<" " <<" timeAmount: " <<(simTime() - firstPktSendTime) <<endl;
  EV << "STAT: " << getFullPath() << " Gen Output BW (B/s):" << oBW  << endl;
}

IBGenerator::~IBGenerator() {
  if (pushMsg) cancelAndDelete(pushMsg);
}

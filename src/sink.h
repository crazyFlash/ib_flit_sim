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
//

#ifndef __SINK_H
#define __SINK_H

#include <omnetpp.h>
using namespace omnetpp;

// we use this to track each message
// 消息元组, 不是已经有了消息的类吗？为什么又整一个消息元组
class MsgTupple {
public:
	unsigned int srcId;     // 源ID
	unsigned int appIdx;    // APP id
	unsigned int msgIdx;    // 消息ID
	MsgTupple(unsigned int s, unsigned int a, unsigned int m) {
		srcId = s; appIdx = a; msgIdx = m;
	};
	std::string dump() const {
		char buff[128];
		sprintf(buff, " src: %d app: %d msg: %d", srcId, appIdx, msgIdx);
		return(std::string(buff));
	};
};

class MsgTuppleLess {
public:
	bool operator()(const MsgTupple &a,const MsgTupple &b) {
		return ((a.srcId < b.srcId) ||  // 源ID小于
				((a.srcId == b.srcId) && (a.appIdx < b.appIdx)) ||  //同源但是APP小于
				((a.srcId == b.srcId) && (a.appIdx == b.appIdx) && (a.msgIdx < b.msgIdx))); // 同源同APP，但是消息ID小于
	}
};

// store msg context
class OutstandingMsgData {
public:
	simtime_t firstFlitTime;
	simtime_t enoughPktsLastFlitTime;
	unsigned int numPktsReceived;

	OutstandingMsgData() { numPktsReceived = 0; };
};

//
// Consumes IB Credits; see NED file for more info.
//
class IBSink : public cSimpleModule
{
 private:
  cMessage *p_hiccupMsg;
  cMessage *p_drainMsg;

  // parameters
  double popDlyPerByte_ns; // the PCI Exp drain rate per byte
  int maxVL;        // Maximum value of VL
  int flitSize;   // size in bytes of single flit
  double startStatCol_sec; // time to start co
  unsigned int repFirstPackets; // if not zero declare a message as done after first repFirstPackets arrived

  // data structure
  int     duringHiccup;                  // set to 1 if during a hiccup
  cQueue  queue;
  simtime_t lastConsumedPakcet;          // the last time a packet was consumed
  unsigned int lid;                      // the HCA LID
  std::map<unsigned int, unsigned int> lastPktSnPerSrc; // last packet serial number per SRC
  // in order to calculate the message latencies we track outstanding messages,
  // first pkt arrival and number of rec packets
  std::map<MsgTupple, class OutstandingMsgData, MsgTuppleLess> outstandingMsgsData;

  // methods
  void newDrainMessage(double delay);  
  void consumeDataMsg(IBDataMsg *p_msg);
  void handlePop(cMessage *p_msg);
  void handleData(IBDataMsg *p_msg);
  void handleHiccup(cMessage *p_msg);

  // statistics
  cHistogram PakcetFabricTime;
  cStdDev waitStats;          // Data Packets Wait Time statistics
  cStdDev hiccupStats;        // statistics about hiccups
  std::vector<int> VlFlits;   // total number of FLITs per VL
  int  AccBytesRcv;           // total number of bytes received
  cOutVector oooPackets;      // vector of number of total OOO packets received
  unsigned int totOOOPackets; // total number of OOO packets received
  cStdDev oooWindow;          // in packets
  unsigned int totOOPackets;  // the total number of packets that need retransmission inc the window
  unsigned int totIOPackets;  // the total packets received in order
  cHistogram msgLatency; // the network latency of received messages from the
                               // time first msg flit was injected to the time last msg flit received
  cHistogram msgF2FLatency; // the network latency of received messages from the
                               // time first msg flit was injected to the time last packet first flit received
  cHistogram enoughPktsLatency; // the network latency of received repFirstPackets of the messages
                                     // from the time first msg flit was injected to the time the last
                                     // flit of the first repFirstPackets was received
  cStdDev enoughToLastPktLatencyStat; // statistics about the time difference from enough pkts to last pkt

 protected:
  virtual void initialize();
  virtual void handleMessage(cMessage *msg);
  virtual void finish();
  virtual ~IBSink();
};

#endif

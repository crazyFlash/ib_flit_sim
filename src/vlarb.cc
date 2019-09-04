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
// The IBVLArb implements an IB VL Arbiter
// See functional description in the header file.
//

/**
 * VLArb 跟IBUF对接
 */

#include "ib_m.h"
#include "vlarb.h"
#include "obuf.h"
#include "ibuf.h"
#include <iomanip>
#define ev EV

using namespace std;

Define_Module( IBVLArb );

void IBVLArb::setVLArbParams(const char *cfgStr, ArbTableEntry *tbl)
{
  int idx = 0;
  char *buf, *p_vl, *p_weight;
  buf = new char[strlen(cfgStr)+1];
  strcpy(buf, cfgStr);
  // 获取各个VL口的 限值
  p_vl = strtok(buf, ":");
  while (p_vl && (idx < 8)) {
    int vl = atoi(p_vl);
    if (vl > 8) {
        throw cRuntimeError("-E- %s VL: %d > 8 in VLA: %s ",
                getFullPath().c_str(), vl, cfgStr);
    }

    int weight;
    p_weight = strtok(NULL, ", ");
    if (! p_weight) {
        throw cRuntimeError("-E- %s badly formatted VLA: %s",
                getFullPath().c_str(), cfgStr);
    }
    weight = atoi(p_weight);
    if (weight > 255) {
        throw cRuntimeError("-E- %s weight: %d > 255 in VLA: %s",
                getFullPath().c_str(), weight, cfgStr);
    }
    
    tbl[idx].VL = vl;
    tbl[idx].weight = weight;
    tbl[idx].used = 0;
    idx++;
    p_vl = strtok(NULL, ":");
  }
   
  // the rest are zeros  还会有the rest 吗
  for (;idx < 8; idx++ ) {
    tbl[idx].VL = 0;
    tbl[idx].weight = 0;
    tbl[idx].used = 0;
  }
  delete [] buf;
}

void IBVLArb::initialize()
{
  int coreFreq_hz = par("coreFreq");
  int    busWidth_B = par("busWidth");
  // read parameters
  hcaArb = par("isHcaArbiter");
  maxVL = par("maxVL");     // 听说这个是max value of the VL,而不是the num of VL
  useFCFSRQArb = par("useFCFSRQArb");

  if (!hcaArb) {
    EV << "-I- " << getFullPath() << " is Switch IBuf " << getId() <<  endl;
    cModule*    sw = getParentModule()->getParentModule();
    VSWDelay = sw->par("VSWDelay");
  }
  setVLArbParams(par("highVLArbEntries"), HighTbl); //设置VL的最大限值
  setVLArbParams(par("lowVLArbEntries"), LowTbl);   //设置VL的最低限值
  vlHighLimit = par("vlHighLimit");
  // The internal bus from in-buf to out-bus is assumed to
  // be clocking every coreFreq cycle and with width of busWidth
  popDelayPerByte_s =  1.0 / busWidth_B / coreFreq_hz;
  ev << "-I- " << getFullPath() << " popDelayPerByte = " << 1e9*popDelayPerByte_s << " [nsec] " << endl;
  WATCH(popDelayPerByte_s);

  // Initiazlize the statistical collection elements
  portXmitWaitHist.setName("Packet Waits for Credits");
//  portXmitWaitHist.setRangeAutoUpper(0, 10, 1);
  portXmitWaitHist.setRange(0, NAN),
  portXmitWaitHist.setNumPrecollectedValues(10);
  portXmitWaitHist.setRangeExtensionFactor(1);

  vl0Credits.setName("free credits on VL0");
  vl1Credits.setName("free credits on VL1");
  readyData.setName("Binary coded VL's with data");
  arbDecision.setName("arbitrated VL");
  
  // Initialize the ready to be sent credit pointers
  numInPorts = gateSize("in");
  unsigned int numSentPorts = gateSize("sent");
  ASSERT(numInPorts == numSentPorts);
  
  // we need a two dimentional array of data packets
  inPktHoqPerVL = new IBDataMsg**[numInPorts];
  for (unsigned int pn = 0; pn < numInPorts; pn++)
    inPktHoqPerVL[pn] = new IBDataMsg*[maxVL+1];
  
  for (unsigned int pn = 0; pn < numInPorts; pn++ ) {
    for (unsigned int vl = 0; vl < maxVL+1; vl++ ) {
      inPktHoqPerVL[pn][vl] = NULL;
      WATCH(inPktHoqPerVL[pn][vl]);
    }
  }
  
  // we also need a two dim array for tracking our promise 
  // to bufs such to avoid a race betwen two requets
  hoqFreeProvided = new short*[numInPorts];
  for (unsigned int pn = 0; pn < numInPorts; pn++)
    hoqFreeProvided[pn] = new short[maxVL+1];
  for (unsigned int pn = 0; pn < numInPorts; pn++ )
    for (unsigned int vl = 0; vl < maxVL+1; vl++ )
      hoqFreeProvided[pn][vl] = 0;
  
  // Init FCCL and FCTBS and the last sent port...
  // 真要命啊，VL这里等于设置为了0，FCCL和FCTBS数组大小都是1
  for (unsigned int vl = 0; vl < maxVL+1; vl++ ) {
    LastSentPort.push_back(0);
    FCTBS.push_back(0);
    FCCL.push_back(0);
  }
  
  WATCH_VECTOR(LastSentPort);
  WATCH_VECTOR(FCTBS);
  WATCH_VECTOR(FCCL);
  
  lastSendTime = 0;
  LastSentVL = 0;
  LastSentWasHigh = 0;
  InsidePacket = 0;
  LowIndex = 0;
  HighIndex = 0;
  SentHighCounter = vlHighLimit*4096/64;
  
  // The pop message is set every time we send a packet
  // when it is not scheduled we are ready for arbitration
  p_popMsg = new cMessage("pop", IB_POP_MSG);
}

// return the FCTBS of the OBUF driven by the VLA
// The hardware does not use this model
int IBVLArb::getOBufFCTBS(unsigned int vl)
{
  cGate *p_gate = gate("out")->getPathEndGate();
  IBOutBuf *p_oBuf = dynamic_cast<IBOutBuf *>(p_gate->getOwnerModule());
  if ((p_oBuf == NULL) || strcmp(p_oBuf->getName(), "obuf")) {
      throw cRuntimeError("-E- %s fail to get OBUF from out port", getFullPath().c_str());
  }
  return(p_oBuf->getFCTBS(vl));
}

// return 1 if the HoQ for that port/VL is free
int IBVLArb::isHoQFree(unsigned int pn, unsigned int vl)
{
//  if ((pn < 0) || (pn >= numInPorts) ) {
  if (pn >= numInPorts)  {
      throw cRuntimeError("-E- %s got out of range port num: %d",
              getFullPath().c_str(), pn);
  }

//  if ( (vl < 0) || (vl >= maxVL+1)) {
  if (vl >= maxVL+1) {
      throw cRuntimeError("-E- %s got out of range vl: %d", getFullPath().c_str(), vl);
  }
  
  // since there might be races between simultanous requests
  // and when they actually send we keep a parallel array 
  // for tracking "free" HoQ returned and clear them on
  // message push
  if ((inPktHoqPerVL[pn][vl] == NULL) && (hoqFreeProvided[pn][vl] == 0)) {
    hoqFreeProvided[pn][vl] = 1;
    return(1);
  }
  return(0);
}

// Sending the message out:
// Update FCTBS
void IBVLArb::sendOutMessage(IBDataMsg *p_msg)
{
  simtime_t delay = p_msg->getByteLength() * popDelayPerByte_s;
  // we can only send if there is no such message as we use
  // it to flag the port is clear to send.
  if ( ! p_popMsg->isScheduled() ) {
    scheduleAt(simTime() + delay, p_popMsg);    //启动自发送pop消息
  } else {
      throw cRuntimeError("-E- %s How can we have two messgaes leaving at the same time",
              getFullPath().c_str());
    return;
  }
  
  // remember if last send was last of packet:
  LastSentWasLast = (p_msg->getFlitSn() + 1 == p_msg->getPacketLength());
  
  if (!hcaArb) {
    simtime_t storeTime = simTime() - p_msg->getArrivalTime();
    simtime_t extraStoreTime = VSWDelay*1e-9 - storeTime;
    if (simTime() + extraStoreTime <= lastSendTime) {
      extraStoreTime = lastSendTime + 1e-9 - simTime();
    }
    if (extraStoreTime > 0) {
      lastSendTime = simTime() + extraStoreTime;
      sendDelayed(p_msg, extraStoreTime, "out");
    } else {
      lastSendTime = simTime();
      send(p_msg, "out");
    }
  } else {
    send(p_msg, "out");
  }
  
  FCTBS[p_msg->getVL()]++;
}

// Notify the IBUF that the flit was sent out
void IBVLArb::sendSentMessage(unsigned int portNum, unsigned int vl)
{
  EV << "-I- " << getFullPath()
     << " informing ibuf with 'sent' message through:" << portNum 
     << " vl:" << vl << " last:" << LastSentWasLast << endl;
  IBSentMsg *p_sentMsg = new IBSentMsg("sent", IB_SENT_MSG);
  p_sentMsg->setVL(vl);
  p_sentMsg->setWasLast(LastSentWasLast);
  hoqFreeProvided[portNum][vl] = 0;

  send(p_sentMsg, "sent", portNum );
}

// An arbitration is valid on two conditions:
// 1. The output port OBUF has free entries
// 2. The IBUF is not already to busy with other ports
int IBVLArb::isValidArbitration(unsigned int portNum, unsigned int vl,
                                int isFirstPacket, int numPacketCredits)
{
  cGate *p_gate = gate("out")->getPathEndGate();
  IBOutBuf *p_oBuf = dynamic_cast<IBOutBuf *>(p_gate->getOwnerModule());
  if ((p_oBuf == NULL) || strcmp(p_oBuf->getName(), "obuf")) {
      throw cRuntimeError("-E- %s fail to get OBUF from out port", getFullPath().c_str());
  }
  
  // check the entire packet an fit in
  int obufFree = p_oBuf->getNumFreeCredits();
  if (isFirstPacket && (obufFree <= numPacketCredits)) {
    EV << "-I- " << getFullPath() 
       << " not enough free OBUF credits:" << obufFree << " requierd:" 
       << numPacketCredits <<" invalid arbitration." << endl;
    return 0;
  }

  // only for non HCA Arbiters and in case of new packet being sent
  if (!hcaArb && isFirstPacket) {
    cGate *p_remOutPort = gate("in", portNum)->getPathStartGate();
    IBInBuf *p_inBuf = dynamic_cast<IBInBuf *>(p_remOutPort->getOwnerModule());
    if ((p_inBuf == NULL) || strcmp(p_inBuf->getName(), "ibuf") ) {
        throw cRuntimeError("-E- %s fail to get InBuf from in port: %d",
                getFullPath().c_str(), portNum);
    }

    if (!p_inBuf->incrBusyUsedPorts()) {  
      EV << "-I- " << getFullPath() 
         << " no free ports on IBUF - invalid arbitration." << endl;
      return 0;
    }
  }
  return 1;
}

// NOTE: If vlHighLimit was reached a single packet
// of the lower table is transmitted.

// find the next port that has data on this VL and not in the middle of
// transmission. return 1 if found
int
IBVLArb::roundRobinNextRQForVL(int numCredits, unsigned int curPortNum, short int vl,
							   int &nextPortNum)
{
	IBDataMsg *p_flit;
	// start with the next port to the last one we sent
    for (unsigned int pn = 1; pn <= numInPorts; pn++) {
      unsigned int portNum = (curPortNum + pn) % numInPorts;
      p_flit = inPktHoqPerVL[portNum][vl];
      // do we have anything to send?
      if (p_flit == NULL)  continue;

      // just make sure it is a first credit

      // we can have another messages leaving at the same time so
      // ignore that port/vl if in the middle of another transfer
      if (p_flit->getFlitSn()) {
        EV << "-I- " << getFullPath() << " ignoring non first packet:"
           << p_flit->getName() << " on port:" << portNum
           << " vl:" << vl << endl;
      } else {
        // so can we send it?
        if (p_flit->getPacketLength() <= numCredits) {
        	nextPortNum = portNum;
        	return(1);
        } else {
          EV << "-I- " << getFullPath() << " not enough credits available:"
             << numCredits << " < " << p_flit->getPacketLength()
             << " required for sending:"
             << p_flit->getName() << " on port:" << portNum
             << " vl:" << vl << endl;
        }
      }
    }
    return(0);
}

// find the next port that has oldest data on this VL and not in the middle of
// transmission. return 1 if found
int
IBVLArb::firstComeFirstServeNextRQForVL(int numCredits, unsigned int curPortNum, short int vl,
							   int &nextPortNum)
{
	IBDataMsg *p_flit;
	IBDataMsg *p_oldestFlit = NULL;
	simtime_t oldestFlitTime;
	int oldestPortNum;

	// start with the next port to the last one we sent
    for (unsigned int pn = 1; pn <= numInPorts; pn++) {
      unsigned int portNum = (curPortNum + pn) % numInPorts;
      p_flit = inPktHoqPerVL[portNum][vl];
      // do we have anything to send?
      if (p_flit == NULL)  continue;

      // just make sure it is a first credit

      // we can have another messages leaving at the same time so
      // ignore that port/vl if in the middle of another transfer
      if (p_flit->getFlitSn()) {
        EV << "-I- " << getFullPath() << " ignoring non first packet:"
           << p_flit->getName() << " on port:" << portNum
           << " vl:" << vl << endl;
      } else {
    	  simtime_t thisFlitTime = p_flit->getSwTimeStamp();
    	  // now look for the oldest
    	  if (!p_oldestFlit || oldestFlitTime > thisFlitTime) {
    		  p_oldestFlit = p_flit;
    		  oldestFlitTime = thisFlitTime;
    		  oldestPortNum = portNum;
    	  }
      }
    }

    if (!p_oldestFlit) return(0);

    // so can we send it?
    if (p_oldestFlit->getPacketLength() <= numCredits) {
    	nextPortNum = oldestPortNum;
    	return(1);
    } else {
      EV << "-I- " << getFullPath() << " not enough credits available:"
         << numCredits << " < " << p_oldestFlit->getPacketLength()
         << " required for sending:"
         << p_oldestFlit->getName() << " on port:" << oldestPortNum
         << " vl:" << vl << endl;
    }

    return(0);
}

// Find the port and VL to be sent next from a High or Low entries.
// Given:
// * current index in the table
// * The table of entries (VL,Weight) pairs
// Return:
// * 1 if found a port to send data from or 0 if nothing to send
// * Update the provided entry index
// * Update the port number
// * update the VL
int
IBVLArb::findNextSend( unsigned int &curIdx, ArbTableEntry *Tbl, 
                       unsigned int &curPortNum, unsigned int &curVl )
{
  int idx;
  short int vl;
  int found = 0;
  int numCredits = 0;
  int portNum;
  
  // we need to scan through all entries starting with last one used
  for (unsigned int i = 0; i <= maxVL+1 ; i++) {
    idx = (curIdx + i) % (maxVL+1);
    // if we changed index we need to restat the weight counter
    if (i) Tbl[idx].used = 0;
    
    // we should skip the entry if it has zero credits (weights) not used
    if (!Tbl[idx].weight || (Tbl[idx].used > Tbl[idx].weight)) continue;
    
    vl = Tbl[idx].VL;
    
    // how many credits are available for this VL
    numCredits = FCCL[vl] - FCTBS[vl];

    if (useFCFSRQArb)
    	found = firstComeFirstServeNextRQForVL(numCredits, curPortNum, vl, portNum);
    else
    	found = roundRobinNextRQForVL(numCredits, curPortNum, vl, portNum);
    if (found)  {
      curIdx = idx;
      curPortNum = portNum;
      curVl = vl;
      break;
    }
  }
  
  return(found);
}

// Find the port on VL0 to send from 
// Given:
// Return:
// * 1 if found a port to send data from or 0 if nothing to send
// * Update the port number
int
IBVLArb::findNextSendOnVL0( unsigned int &curPortNum )
{
  int found = 0;
  int numCredits = 0;
  int portNum;
  IBDataMsg *p_flit;
  
  // how many credits are available for this VL
  numCredits = FCCL[0] - FCTBS[0];
  
  // start with the next port to the last one we sent
  for (unsigned int pn = 1; pn <= numInPorts; pn++) {
    portNum = (curPortNum + pn) % numInPorts;
    p_flit = inPktHoqPerVL[portNum][0];
    // do we have anything to send?
    if (p_flit == NULL)  continue;
    
    // just make sure it is a first credit
    
    // we can have another messages leaving at the same time so
    // ignore that port/vl if in the middle of another transfer
    if (p_flit->getFlitSn()) {
      EV << "-I- " << getFullPath() << " ignoring non first packet:"
         << p_flit->getName() << " on port:" << portNum
         << " vl:" << 0 << endl;
    } else {
      // so can we send it?
      if (p_flit->getPacketLength() <= numCredits) {
        found = 1;
        break;
      } else {
        EV << "-I- " << getFullPath() << " not enough credits available:"
           << numCredits << " < " << p_flit->getPacketLength() 
           << " required for sending:"
           << p_flit->getName() << " on port:" << portNum
           << " vl:" << 0 << endl;
      }
    }
  }
  
  if (found)
    curPortNum = portNum;
  return(found);
}

// Display the internal state for debug purposes
void IBVLArb::displayState()
{
  // print the state of the arbiter
  if (cLog::logLevel < LOGLEVEL_INFO) {
    ev << "-I- " << getFullPath() << " ARBITER STATE as VL/Used/Weight" 
       << endl;
    ev << "-I- High:";
    for (unsigned int e = 0; e < maxVL+1; e++) {
      if (LastSentWasHigh && HighIndex == e)
        ev << "*" << HighTbl[e].VL << " " 
           << setw(3) << HighTbl[e].used
           << "/" << setw(3) << HighTbl[e].weight << "*";
      else
        ev << "|" << HighTbl[e].VL << " " 
           << setw(3) << HighTbl[e].used
           << "/" << setw(3) << HighTbl[e].weight << " ";
    }
    if (LastSentWasHigh)
      ev << "<----" << SentHighCounter << endl;
    else
      ev << endl;
    
    ev << "-I- Low: ";
    for (unsigned int e = 0; e < maxVL+1; e++) {
      if (!LastSentWasHigh && LowIndex == e)
        ev << "*" << LowTbl[e].VL << " " 
           << setw(3) << LowTbl[e].used
           << "/" << setw(3) << LowTbl[e].weight << "*";
      else
        ev << "|" << LowTbl[e].VL << " " 
           << setw(3) << LowTbl[e].used
           << "/" << setw(3) << LowTbl[e].weight << " ";
    }
    ev << endl;
  }
  
  int vlsWithData = 0;
  for (unsigned int vl = 0; vl < maxVL+1; vl++) {
    int fctbs = FCTBS[vl];
    int freeCredits = FCCL[vl] - fctbs;
    EV << "-I- " << getFullPath() << " vl:" << vl
       << " " << FCCL[vl] << "-" << fctbs << "=" 
       << freeCredits << " Ports " ;
    int anyInput = 0;
    for (unsigned int pn = 0; pn < numInPorts ; pn++) {
      if (inPktHoqPerVL[pn][vl]) {
        anyInput = 1;
        EV << pn << ":Y ";
      } else {
        EV << pn << ":n ";
      }
    }
    
    EV << endl; 
    if (anyInput)
      vlsWithData |= 1<<vl;
    
    if (vl == 0)
      vl0Credits.record(freeCredits);
    else if (vl == 1)
      vl1Credits.record(freeCredits);
  }
  readyData.record(10*vlsWithData);
}

// Arbitration:
//
// Decides which data to send and provide back a "sent" notification.
//
// Data Structure:
// HighIndex, LowIndex - points to the index in the VLArb tables.
// LastSentPort[VL] - points to the last port that have sent data on a VL
// SentHighCounter - counts how many credits still needs to be sent from high
// LastSentWasHigh - let us know if we were previously sending from
//    low or high table
// inPktHoqPerVL[pn][vl] - an array per port and VL pointing to first
//    flit of the packet
//
// NOTE: As we decide to arbitrate on complete packets only we might need to
//       increase the credit count if what is left is smaller then the number
//       of credits the packet carries.
//
// Algorithm:
// NOTE: as we only need to decide on a packet boundary we keep track of the
// last port and vl used and simply use all credits from it.
// * If the SentHighCounter is 0 then we use LowTable otherwise HighTable
// * Find the first port, after the last one that was sent, that has data
//   ready to send on the current index vl.
// * If not found incr index and repeat until all entries
//   searched (ignore 0 weight entries).
// * If found a new index use it and update the credits sent vs the weight.
// * If not found and we are HighTable - use LowTable and do as above.
// * Dec the SentHighCounter if we are HighTable. Load it to vlHighLimit*4k/64
//   otherwise.
//
void IBVLArb::arbitrate()
{
  int found = 0;
  IBDataMsg *nextSendHoq;
  int isFirstPacket = 0;
  int isLastFlit;
  unsigned int portNum;
  unsigned int vl;
  
  // can not arbitrate if we are in a middle of send
  if (p_popMsg->isScheduled()) {
    EV << "-I- " << getFullPath() 
       << " can not arbitrate while packet is being sent" << endl;
    return;
  }

  // display arbiter state and receovd vectors...
  displayState();

  // if we did not reach the end of the current packet simply send this credit
  if (InsidePacket) {
    vl = LastSentVL;
    portNum = LastSentPort[vl];
    
    nextSendHoq = inPktHoqPerVL[portNum][vl];
    if (! nextSendHoq) { 
      EV << "-I- " << getFullPath() << " HoQ empty for port:"
         << portNum << " VL:" << vl << endl;
      return;
    }
    
    isLastFlit = 
      (nextSendHoq->getFlitSn() + 1 == nextSendHoq->getPacketLength());
    
    if (isLastFlit) {
      EV << "-I- " << getFullPath() << " sending last credit packet:"
         << nextSendHoq->getName() << " from port:" << portNum
         << " vl:" <<  vl << endl; 
      InsidePacket = 0;
    } else {
      EV << "-I- " << getFullPath() << " sending continuation credit packet:"
         << nextSendHoq->getName() << " from port:" << portNum
         << " vl:" << vl << endl;
    }
    
    // need to decrement the SentHighCounter if we are sending high packets
    if (LastSentWasHigh) {
      SentHighCounter--;
    } else {
      // If we are sending the last credit of packet when the SentHighCounter
      // is zero we need to reload it as this was the last credit of forced low
      // packet
      if ( isLastFlit && (SentHighCounter == 0))
        SentHighCounter = vlHighLimit*4096/64;
    }
    found = 1;
  } else {
    // not inside a packet so need to arbitrate a new one
    isFirstPacket = 1;
    
    // IF WE ARE HERE WE NEED TO FIND FIRST PACKET TO SEND
    portNum = LastSentPort[LastSentVL];
    vl = LastSentVL;
    
    if (maxVL > 0) {
      // if we are in High Limit case try first from low
      if ( SentHighCounter <= 0 ) {
        found = 1;
        if (findNextSend(LowIndex, LowTbl, portNum, vl))
          LastSentWasHigh = 0;
        else if ( findNextSend(HighIndex, HighTbl, portNum, vl) )
          LastSentWasHigh = 1; 
        else
          found = 0;
      } else { 
        found = 1;
        if ( findNextSend(HighIndex, HighTbl, portNum, vl) )
          LastSentWasHigh = 1; 
        else if (findNextSend(LowIndex, LowTbl, portNum, vl))
          LastSentWasHigh = 0;
        else
          found = 0;
      }
    } else {
      vl = 0;
      found = findNextSendOnVL0(portNum);
    }
    
    if (found) {
      if (LastSentWasHigh) {
        EV << "-I- " << getFullPath() << " Result High idx:" 
           << HighIndex << " vl:" << vl
           << " port:" << portNum << " used:" << HighTbl[HighIndex].used
           << " weight:" << HighTbl[HighIndex].weight
           << " high count:" << SentHighCounter << endl;
      } else {
        EV << "-I- " << getFullPath() << " Result Low idx:" 
           << LowIndex << " vl:" << vl
           << " port:" << portNum << " used:" << LowTbl[LowIndex].used
           << " weight:" << LowTbl[LowIndex].weight << endl;
      }
      
      nextSendHoq = inPktHoqPerVL[portNum][vl];
      isLastFlit = 
        (nextSendHoq->getFlitSn() + 1 == nextSendHoq->getPacketLength());
      
      if (!isLastFlit) {
        InsidePacket = 1; 
        EV << "-I- " << getFullPath() << " sending first credit packet:"
           << nextSendHoq->getName() << " from port:" << portNum
           << " vl:" << vl << endl; 
      } else { 
        InsidePacket = 0;
        EV << "-I- " << getFullPath() << " sending single credit packet:"
           << nextSendHoq->getName() << " from port:" << portNum
           << " vl:" << vl << endl; 
      }
    } else {
      EV << "-I- " << getFullPath() << " nothing to send" <<endl;
      arbDecision.record(-1);
      return;
    }
  } // first or not
  
  // we could arbitrate an invalid selection due to lack of output Q
  // or busy ports of the input port we want to arbitrate.
  if (isValidArbitration(portNum, vl, isFirstPacket, 
                         nextSendHoq->getPacketLength())) {
    // do the actual send and record our successful arbitration
    if (nextSendHoq->getFlitSn() == 0) {
      LastSentVL = vl;
      LastSentPort[vl] = portNum;
    }
    
    inPktHoqPerVL[LastSentPort[LastSentVL]][LastSentVL] = NULL;
    
    if (LastSentWasHigh)
      HighTbl[HighIndex].used ++;
    else
      LowTbl[LowIndex].used ++;
    
    arbDecision.record(10*(vl+1));
    sendOutMessage(nextSendHoq);
    sendSentMessage(LastSentPort[LastSentVL],LastSentVL);
  } else {
    // if we are in the first data credit cleanup the InsidePacket flag
    if (nextSendHoq->getFlitSn() == 0)
      InsidePacket = 0;
    arbDecision.record(-1);
  }
} // arbitrate

// Handle push message
// A new credit is being provided by some port
// We need to know which port we get the data on,
// which VL is it on
// if the HOQ is aleady taken assert
/**
 * push型消息，也是data型？
 * 收到了一个新的credit,需要知道他是在哪个端口，哪个VL上
 */
void IBVLArb::handlePush(IBDataMsg *p_msg)
{
  // what port did we get it from ?
  /*unsigned*/ int pn = p_msg->getArrivalGate()->getIndex();    //获取接收数据的端口号
  unsigned short int vl = p_msg->getVL();                   // 获取接收数据端口的VL号
  if ((pn < 0) || (pn >= numInPorts) ) {
      throw cRuntimeError("-E- %s got out of range port num: %d",
              getFullPath().c_str(), pn);
  }

  // 超出规定VL数值
  if (vl >= maxVL+1) {
      throw cRuntimeError("-E- %s VLA got out of range vl: %d by %s, arrived at port %d",
              getFullPath().c_str(), vl, p_msg->getName(), pn); 
  }
  
  // inPktHopPerVL: the head of the send Q on every VL
  if (inPktHoqPerVL[pn][vl] != NULL) {
      throw cRuntimeError("-E- %s Overwriting HoQ port: %d by %s arrived at port: %d",
              getFullPath().c_str(), pn, p_msg->getName(), pn);
  }
  
  // hopFreeProvided: set when a "free" HoQ provided/cleared on push
  if (hoqFreeProvided[pn][vl] == 0) {
      throw cRuntimeError("-E- %s No previous HoQ free port: %d VL: %d by %s at port: %d",
              getFullPath().c_str(), pn, vl, p_msg->getName(), pn);
  }
  
  EV << "-I- " << getFullPath() << " filled HoQ for port:"
     << pn << " vl:" << vl << " with:" << p_msg->getName() <<  endl;
  
  inPktHoqPerVL[pn][vl] = p_msg;
  hoqFreeProvided[pn][vl] = 0;
  arbitrate();
}

// Handle Pop Message
// clear the pop message and send the "sent message" then try to arbitrate
//
// NOTE: Only now when the packet was sent we can tell the IBUF
// to free its credits and busy ports.
/**
 * 数据包已经发出，向IBUF发送Sent message。
 */
void IBVLArb::handlePop()
{
  cancelEvent(p_popMsg);    //什么时候启动的这个pop消息
  arbitrate();  //仲裁？公断？
}

// Handle TxCred
/**
 * 接收目的端口的消息，更新相应端口VL的数值
 */
void IBVLArb::handleTxCred(IBTxCredMsg *p_msg)
{
  int vl = p_msg->getVL();
  // update FCCL...
  FCCL[vl] = p_msg->getFCCL();
  
  EV << "-I- " << getFullPath() << " updated vl:" << vl
     << " fccl:" << p_msg->getFCCL()
     << " can send :" << FCCL[vl] - FCTBS[vl] << endl;
  
  delete p_msg;
  arbitrate();
}

// VLArb 收到消息处理
void IBVLArb::handleMessage(cMessage *p_msg)
{
  int msgType = p_msg->getKind();
  if ( msgType == IB_POP_MSG ) {    //pop 类型
    handlePop();
  } else if ( msgType == IB_DATA_MSG ) {    //data类型数据，Push？？？为什么不是handledata
    handlePush((IBDataMsg*)p_msg);
  } else if ( msgType == IB_TXCRED_MSG ) {  //Txcred 什么类型。。。
    handleTxCred((IBTxCredMsg*)p_msg);
  } else if ( (msgType == IB_DONE_MSG) || (msgType == IB_FREE_MSG) ) {  //done 型或者FREE 类型
    delete p_msg;
    arbitrate();
  } else {
      throw cRuntimeError("-E- %s does not know how to handle message: %d",
              getFullPath().c_str(), msgType);
    delete p_msg;
  }
}

void IBVLArb::finish()
{
  /*ev << "STAT: " << getFullPath() << " Wait for credits num/avg/max/std "
    << portXmitWaitHist.getCount() 
    << " / " << portXmitWaitHist.getMean()
    << " / " << portXmitWaitHist.getMax() 
    << " / " << portXmitWaitHist.getStddev() << endl;
  */
}

IBVLArb::~IBVLArb() {
	if (p_popMsg) cancelAndDelete(p_popMsg);
}

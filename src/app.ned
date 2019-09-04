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
package ib_model;

simple IBApp
{
 parameters:
  // - destination 
  string dstMode; // @enum = (param|seq_once|seq_loop|seq_rand) // mode
  volatile int dstLid; // the destination LID - used in DST_PARAM mode
  string dstSeqVecFile;// the vector file name that contain the sequences
  int dstSeqVecIdx; // the index of the generator in the file
  
  // - length 
  string msgLenMode; //  @enum = (param|set); // possible values: param|set
  volatile int msgMtuLen @unit(B); // msg packets MTU
  volatile int msgLength @unit(B); // the length of a message
  string msgLenSet; // a set of lengths in bytes e.g. "1152 256 128 2048"
  string msgLenProb; // probability of each length e.g. "0.1 0.1 0.2 0.6"
  
  // - shape
  volatile double msg2msgGap @unit(ns); // extra delay between messages
  
  // - SQ
  volatile int msgSQ;
  @display("i=block/telnet");
 gates:
  inout out;
}

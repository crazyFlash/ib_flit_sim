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

//
// This NED provides an InfiniBand Wire Channel model

channel IBWire extends ned.DatarateChannel
{
      double width;                 // the output link width ,4,8,12 lanes
      double speed @unit(Gbps);    // the output speed 2.5, 5.0, 10.0
      double encode;
      
      datarate = width * speed * encode; // Disparity - 8b/10b encoding
}

//
// This NED provides several InfiniBand  Wire Channel model
channel IB4XEDRWire extends IBWire
{
      width = 4;                 // the output link width
      speed = 25.0Gbps;          // the output speed
      encode = 64.0/66;	//  64b/66b encoding
}

channel IB4XFDRWire extends IBWire
{
      width = 4;                 // the output link width
      speed = 14.0Gbps;          // the output speed
      
//      encode = 0.8;	//  64b/66b encoding
      encode = 64.0/66;	//  64b/66b encoding
}

// before FDR, encoding 8b/10b, after become 64b/66b
channel IB4XQDRWire extends IBWire
{
      width = 4;                 // the output link width
      speed = 5.0Gbps;            // the output speed
      
      encode = 0.8;
}

channel IB4XDDRWire extends IBWire
{
      width = 4;                 // the output link width
      speed = 5.0Gbps;           // the output speed
      
      encode = 0.8;
}

channel IB4XSDRWire extends IBWire
{
      width = 4;                 // the output link width
      speed = 2.5Gbps;           // the output speed
      
      encode = 0.8;
}

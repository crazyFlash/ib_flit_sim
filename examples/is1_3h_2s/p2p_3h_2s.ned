package ib_model.examples.is1_3h_2s;

import ib_model.*;

network p2p_3h_2s
{
    submodules:
        H_1: HCA {
            parameters: srcLid = 1;
            @display("i=device/server2;p=80,109");
        }
        H_2: HCA {
            parameters: srcLid = 2;
            @display("i=device/server2;p=80,191");
        }
        H_3: HCA {
            parameters: srcLid = 3;
            @display("i=device/server2;p=334,144");
        }

        SW_1: Switch {
            parameters:
                numSwitchPorts = 2;
                @display("i=abstract/router;p=257,144");
            gates:
                port[2];
        }
        SW_2: Switch {
            parameters:
                numSwitchPorts = 3;
                @display("i=abstract/router;p=173,144");
            gates:
                port[3];
        }
    connections:
        H_1.port <--> IB4XFDRWire <--> SW_2.port[0];
        H_2.port <--> IB4XFDRWire <--> SW_2.port[1];
        SW_2.port[2] <--> IB4XFDRWire <--> SW_1.port[1];
        SW_1.port[0] <--> IB4XFDRWire <--> H_3.port;
}


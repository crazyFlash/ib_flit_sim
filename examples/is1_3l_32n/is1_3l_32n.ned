package ib_model.examples.is1_3l_32n;

import ib_model.*;

network is1_3l_32n
{
	submodules:
		H_1_U1: HCA { parameters: srcLid = 1; }
		H_10_U1: HCA { parameters: srcLid = 10; }
		H_11_U1: HCA { parameters: srcLid = 11; }
		H_12_U1: HCA { parameters: srcLid = 12; }
		H_13_U1: HCA { parameters: srcLid = 13; }
		H_14_U1: HCA { parameters: srcLid = 14; }
		H_15_U1: HCA { parameters: srcLid = 15; }
		H_16_U1: HCA { parameters: srcLid = 16; }
		H_17_U1: HCA { parameters: srcLid = 17; }
		H_18_U1: HCA { parameters: srcLid = 18; }
		H_19_U1: HCA { parameters: srcLid = 19; }
		H_2_U1: HCA { parameters: srcLid = 2; }
		H_20_U1: HCA { parameters: srcLid = 20; }
		H_21_U1: HCA { parameters: srcLid = 21; }
		H_22_U1: HCA { parameters: srcLid = 22; }
		H_23_U1: HCA { parameters: srcLid = 23; }
		H_24_U1: HCA { parameters: srcLid = 24; }
		H_25_U1: HCA { parameters: srcLid = 25; }
		H_26_U1: HCA { parameters: srcLid = 26; }
		H_27_U1: HCA { parameters: srcLid = 27; }
		H_28_U1: HCA { parameters: srcLid = 28; }
		H_29_U1: HCA { parameters: srcLid = 29; }
		H_3_U1: HCA { parameters: srcLid = 3; }
		H_30_U1: HCA { parameters: srcLid = 30; }
		H_31_U1: HCA { parameters: srcLid = 31; }
		H_32_U1: HCA { parameters: srcLid = 32; }
		H_4_U1: HCA { parameters: srcLid = 4; }
		H_5_U1: HCA { parameters: srcLid = 5; }
		H_6_U1: HCA { parameters: srcLid = 6; }
		H_7_U1: HCA { parameters: srcLid = 7; }
		H_8_U1: HCA { parameters: srcLid = 8; }
		H_9_U1: HCA { parameters: srcLid = 9; }
		SW_L0_0_0_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L0_0_1_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L0_1_0_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L0_1_1_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L1_0_0_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L1_0_1_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L1_1_0_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L1_1_1_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L1_2_0_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L1_2_1_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L1_3_0_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L1_3_1_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L2_0_0_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L2_0_1_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L2_1_0_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L2_1_1_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L2_2_0_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L2_2_1_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L2_3_0_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
		SW_L2_3_1_U1: Switch {
		parameters: numSwitchPorts = 8;
			gates: port[8];
		}
	connections:
		H_1_U1.port <--> IB4XQDRWire <--> SW_L2_0_0_U1.port[0];
		H_10_U1.port <--> IB4XQDRWire <--> SW_L2_1_0_U1.port[1];
		H_11_U1.port <--> IB4XQDRWire <--> SW_L2_1_0_U1.port[2];
		H_12_U1.port <--> IB4XQDRWire <--> SW_L2_1_0_U1.port[3];
		H_13_U1.port <--> IB4XQDRWire <--> SW_L2_1_1_U1.port[0];
		H_14_U1.port <--> IB4XQDRWire <--> SW_L2_1_1_U1.port[1];
		H_15_U1.port <--> IB4XQDRWire <--> SW_L2_1_1_U1.port[2];
		H_16_U1.port <--> IB4XQDRWire <--> SW_L2_1_1_U1.port[3];
		H_17_U1.port <--> IB4XQDRWire <--> SW_L2_2_0_U1.port[0];
		H_18_U1.port <--> IB4XQDRWire <--> SW_L2_2_0_U1.port[1];
		H_19_U1.port <--> IB4XQDRWire <--> SW_L2_2_0_U1.port[2];
		H_2_U1.port <--> IB4XQDRWire <--> SW_L2_0_0_U1.port[1];
		H_20_U1.port <--> IB4XQDRWire <--> SW_L2_2_0_U1.port[3];
		H_21_U1.port <--> IB4XQDRWire <--> SW_L2_2_1_U1.port[0];
		H_22_U1.port <--> IB4XQDRWire <--> SW_L2_2_1_U1.port[1];
		H_23_U1.port <--> IB4XQDRWire <--> SW_L2_2_1_U1.port[2];
		H_24_U1.port <--> IB4XQDRWire <--> SW_L2_2_1_U1.port[3];
		H_25_U1.port <--> IB4XQDRWire <--> SW_L2_3_0_U1.port[0];
		H_26_U1.port <--> IB4XQDRWire <--> SW_L2_3_0_U1.port[1];
		H_27_U1.port <--> IB4XQDRWire <--> SW_L2_3_0_U1.port[2];
		H_28_U1.port <--> IB4XQDRWire <--> SW_L2_3_0_U1.port[3];
		H_29_U1.port <--> IB4XQDRWire <--> SW_L2_3_1_U1.port[0];
		H_3_U1.port <--> IB4XQDRWire <--> SW_L2_0_0_U1.port[2];
		H_30_U1.port <--> IB4XQDRWire <--> SW_L2_3_1_U1.port[1];
		H_31_U1.port <--> IB4XQDRWire <--> SW_L2_3_1_U1.port[2];
		H_32_U1.port <--> IB4XQDRWire <--> SW_L2_3_1_U1.port[3];
		H_4_U1.port <--> IB4XQDRWire <--> SW_L2_0_0_U1.port[3];
		H_5_U1.port <--> IB4XQDRWire <--> SW_L2_0_1_U1.port[0];
		H_6_U1.port <--> IB4XQDRWire <--> SW_L2_0_1_U1.port[1];
		H_7_U1.port <--> IB4XQDRWire <--> SW_L2_0_1_U1.port[2];
		H_8_U1.port <--> IB4XQDRWire <--> SW_L2_0_1_U1.port[3];
		H_9_U1.port <--> IB4XQDRWire <--> SW_L2_1_0_U1.port[0];
		SW_L0_0_0_U1.port[0] <--> IB4XQDRWire <--> SW_L1_0_0_U1.port[4];
		SW_L0_0_0_U1.port[1] <--> IB4XQDRWire <--> SW_L1_0_0_U1.port[5];
		SW_L0_0_0_U1.port[2] <--> IB4XQDRWire <--> SW_L1_1_0_U1.port[4];
		SW_L0_0_0_U1.port[3] <--> IB4XQDRWire <--> SW_L1_1_0_U1.port[5];
		SW_L0_0_0_U1.port[4] <--> IB4XQDRWire <--> SW_L1_2_0_U1.port[4];
		SW_L0_0_0_U1.port[5] <--> IB4XQDRWire <--> SW_L1_2_0_U1.port[5];
		SW_L0_0_0_U1.port[6] <--> IB4XQDRWire <--> SW_L1_3_0_U1.port[4];
		SW_L0_0_0_U1.port[7] <--> IB4XQDRWire <--> SW_L1_3_0_U1.port[5];
		SW_L0_0_1_U1.port[0] <--> IB4XQDRWire <--> SW_L1_0_1_U1.port[4];
		SW_L0_0_1_U1.port[1] <--> IB4XQDRWire <--> SW_L1_0_1_U1.port[5];
		SW_L0_0_1_U1.port[2] <--> IB4XQDRWire <--> SW_L1_1_1_U1.port[4];
		SW_L0_0_1_U1.port[3] <--> IB4XQDRWire <--> SW_L1_1_1_U1.port[5];
		SW_L0_0_1_U1.port[4] <--> IB4XQDRWire <--> SW_L1_2_1_U1.port[4];
		SW_L0_0_1_U1.port[5] <--> IB4XQDRWire <--> SW_L1_2_1_U1.port[5];
		SW_L0_0_1_U1.port[6] <--> IB4XQDRWire <--> SW_L1_3_1_U1.port[4];
		SW_L0_0_1_U1.port[7] <--> IB4XQDRWire <--> SW_L1_3_1_U1.port[5];
		SW_L0_1_0_U1.port[0] <--> IB4XQDRWire <--> SW_L1_0_0_U1.port[6];
		SW_L0_1_0_U1.port[1] <--> IB4XQDRWire <--> SW_L1_0_0_U1.port[7];
		SW_L0_1_0_U1.port[2] <--> IB4XQDRWire <--> SW_L1_1_0_U1.port[6];
		SW_L0_1_0_U1.port[3] <--> IB4XQDRWire <--> SW_L1_1_0_U1.port[7];
		SW_L0_1_0_U1.port[4] <--> IB4XQDRWire <--> SW_L1_2_0_U1.port[6];
		SW_L0_1_0_U1.port[5] <--> IB4XQDRWire <--> SW_L1_2_0_U1.port[7];
		SW_L0_1_0_U1.port[6] <--> IB4XQDRWire <--> SW_L1_3_0_U1.port[6];
		SW_L0_1_0_U1.port[7] <--> IB4XQDRWire <--> SW_L1_3_0_U1.port[7];
		SW_L0_1_1_U1.port[0] <--> IB4XQDRWire <--> SW_L1_0_1_U1.port[6];
		SW_L0_1_1_U1.port[1] <--> IB4XQDRWire <--> SW_L1_0_1_U1.port[7];
		SW_L0_1_1_U1.port[2] <--> IB4XQDRWire <--> SW_L1_1_1_U1.port[6];
		SW_L0_1_1_U1.port[3] <--> IB4XQDRWire <--> SW_L1_1_1_U1.port[7];
		SW_L0_1_1_U1.port[4] <--> IB4XQDRWire <--> SW_L1_2_1_U1.port[6];
		SW_L0_1_1_U1.port[5] <--> IB4XQDRWire <--> SW_L1_2_1_U1.port[7];
		SW_L0_1_1_U1.port[6] <--> IB4XQDRWire <--> SW_L1_3_1_U1.port[6];
		SW_L0_1_1_U1.port[7] <--> IB4XQDRWire <--> SW_L1_3_1_U1.port[7];
		SW_L1_0_0_U1.port[0] <--> IB4XQDRWire <--> SW_L2_0_0_U1.port[4];
		SW_L1_0_0_U1.port[1] <--> IB4XQDRWire <--> SW_L2_0_0_U1.port[5];
		SW_L1_0_0_U1.port[2] <--> IB4XQDRWire <--> SW_L2_0_1_U1.port[4];
		SW_L1_0_0_U1.port[3] <--> IB4XQDRWire <--> SW_L2_0_1_U1.port[5];
		SW_L1_0_1_U1.port[0] <--> IB4XQDRWire <--> SW_L2_0_0_U1.port[6];
		SW_L1_0_1_U1.port[1] <--> IB4XQDRWire <--> SW_L2_0_0_U1.port[7];
		SW_L1_0_1_U1.port[2] <--> IB4XQDRWire <--> SW_L2_0_1_U1.port[6];
		SW_L1_0_1_U1.port[3] <--> IB4XQDRWire <--> SW_L2_0_1_U1.port[7];
		SW_L1_1_0_U1.port[0] <--> IB4XQDRWire <--> SW_L2_1_0_U1.port[4];
		SW_L1_1_0_U1.port[1] <--> IB4XQDRWire <--> SW_L2_1_0_U1.port[5];
		SW_L1_1_0_U1.port[2] <--> IB4XQDRWire <--> SW_L2_1_1_U1.port[4];
		SW_L1_1_0_U1.port[3] <--> IB4XQDRWire <--> SW_L2_1_1_U1.port[5];
		SW_L1_1_1_U1.port[0] <--> IB4XQDRWire <--> SW_L2_1_0_U1.port[6];
		SW_L1_1_1_U1.port[1] <--> IB4XQDRWire <--> SW_L2_1_0_U1.port[7];
		SW_L1_1_1_U1.port[2] <--> IB4XQDRWire <--> SW_L2_1_1_U1.port[6];
		SW_L1_1_1_U1.port[3] <--> IB4XQDRWire <--> SW_L2_1_1_U1.port[7];
		SW_L1_2_0_U1.port[0] <--> IB4XQDRWire <--> SW_L2_2_0_U1.port[4];
		SW_L1_2_0_U1.port[1] <--> IB4XQDRWire <--> SW_L2_2_0_U1.port[5];
		SW_L1_2_0_U1.port[2] <--> IB4XQDRWire <--> SW_L2_2_1_U1.port[4];
		SW_L1_2_0_U1.port[3] <--> IB4XQDRWire <--> SW_L2_2_1_U1.port[5];
		SW_L1_2_1_U1.port[0] <--> IB4XQDRWire <--> SW_L2_2_0_U1.port[6];
		SW_L1_2_1_U1.port[1] <--> IB4XQDRWire <--> SW_L2_2_0_U1.port[7];
		SW_L1_2_1_U1.port[2] <--> IB4XQDRWire <--> SW_L2_2_1_U1.port[6];
		SW_L1_2_1_U1.port[3] <--> IB4XQDRWire <--> SW_L2_2_1_U1.port[7];
		SW_L1_3_0_U1.port[0] <--> IB4XQDRWire <--> SW_L2_3_0_U1.port[4];
		SW_L1_3_0_U1.port[1] <--> IB4XQDRWire <--> SW_L2_3_0_U1.port[5];
		SW_L1_3_0_U1.port[2] <--> IB4XQDRWire <--> SW_L2_3_1_U1.port[4];
		SW_L1_3_0_U1.port[3] <--> IB4XQDRWire <--> SW_L2_3_1_U1.port[5];
		SW_L1_3_1_U1.port[0] <--> IB4XQDRWire <--> SW_L2_3_0_U1.port[6];
		SW_L1_3_1_U1.port[1] <--> IB4XQDRWire <--> SW_L2_3_0_U1.port[7];
		SW_L1_3_1_U1.port[2] <--> IB4XQDRWire <--> SW_L2_3_1_U1.port[6];
		SW_L1_3_1_U1.port[3] <--> IB4XQDRWire <--> SW_L2_3_1_U1.port[7];
}


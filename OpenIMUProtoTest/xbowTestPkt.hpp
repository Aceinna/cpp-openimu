/** ***************************************************************************
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 *  main is the center of the system universe, it all starts here. And never ends.
 * entry point for system (pins, clocks, interrupts), data and task initialization.
 * contains the main processing loop. - this is a standard implementation
 * which has mainly os functionality in the main loop
 ******************************************************************************/
/*******************************************************************************
Copyright 2018 ACEINNA, INC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/
/**
* Classes that define the various Xbow Angle packets
*
* @file    xbowTestPkt.hpp
*/

#ifndef _XBOW_TEST_PKT_HPP_
#define _XBOW_TEST_PKT_HPP_

#include "xbowPacket.hpp"
#include "packetDefns.h"
#include "nfDataTypes.hpp"

/**
* The Test3 packet
*/
class XbowTest3Packet : public XbowPacket
{
  public:
    uint16_t _t3Status[7];
    
    XbowTest3Packet(system_data_t *systemData) : XbowPacket(XBOW_PROTOCOL, T3, 14) {
        _t3Status[0] = systemData->bitWord.all;
        _t3Status[1] = systemData->hwBitWord;
        _t3Status[2] = systemData->swBitWord;
        _t3Status[3] = systemData->sensorsBitWord;
        _t3Status[4] = systemData->hwStatusWord;
        _t3Status[5] = systemData->swStatusWord;
        _t3Status[6] = systemData->sensorsStatusWord;
    }

	XbowTest3Packet(uint8_t *bytes, int payloadLen) : XbowPacket(XBOW_PROTOCOL, T3, 14) {
		_dPtr = bytes;
		for (int i = 0; i < 7; i++){
			ntohb_16(&_t3Status[i]);
		}

	}

    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        for (int i=0; i<7; i++) {
            htonb_16(_t3Status[i]);
        }
                     
        return ESUCCESS;
    }
    
};

class XbowTest1Packet : public XbowPacket
{
public:
	uint16_t _t1Status[16];

	XbowTest1Packet(uint8_t *bytes, int payloadLen) : XbowPacket(XBOW_PROTOCOL, T3, 14) {
		_dPtr = bytes;
		for (int i = 0; i < 16; i++) {
			ntohb_16(&_t1Status[i]);
		}

	}
};


#endif  // _XBOW_TEST_PKT_HPP_

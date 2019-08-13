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
* @file    xbowAnglePkt.hpp
*/

#ifndef _XBOW_ANGLE_PKT_HPP_
#define _XBOW_ANGLE_PKT_HPP_

#include "xbowPacket.hpp"
#include "packetDefns.h"
#include "nfDataTypes.hpp"

#define A6_SCALING_1 1.043037835047045e+04 //  2^15 / PI
#define A6_SCALING_2 2.086075670094091e+04 //  2^15 / (PI/2)
#define A6_SCALING_3 4.172151340188181e+04 //  2^16 / (PI/2)

/**
* The A6 packet
*/
class XbowAngle1Packet : public XbowPacket
{
  public:
    int16_t  _attitude[3];
    int16_t  _rates[3];
	int16_t  _accels[3];
	int16_t  _mags[3];
	int16_t  _temp;
	uint32_t _ITOW;
    uint16_t _bitWord;
    
	XbowAngle1Packet(uint8_t *bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, A1, 32) {
		_dPtr = bytes;
		ntohb_16((uint16_t*)&_attitude[0]);
		ntohb_16((uint16_t*)&_attitude[1]);
		ntohb_16((uint16_t*)&_attitude[2]);
		ntohb_16((uint16_t*)&_rates[0]);
		ntohb_16((uint16_t*)&_rates[1]);
		ntohb_16((uint16_t*)&_rates[2]);
		ntohb_16((uint16_t*)&_accels[0]);
		ntohb_16((uint16_t*)&_accels[1]);
		ntohb_16((uint16_t*)&_accels[2]);
		ntohb_16((uint16_t*)&_mags[0]);
		ntohb_16((uint16_t*)&_mags[1]);
		ntohb_16((uint16_t*)&_mags[2]);
		ntohb_16((uint16_t*)&_temp);
		ntohb_32((uint32_t*)&_ITOW);
		ntohb_16((uint16_t*)&_bitWord);
	}


};


/**
* The DMC packet
*/
class XbowDMCDataPacket : public XbowPacket
{
  public:
    int16_t _attitude[3];
    uint32_t _sysTime;
    uint16_t _bitWord;
    
    XbowDMCDataPacket(double magAttitude[3], system_data_t *sysData) : XbowPacket(XBOW_PROTOCOL, MC, 12) {
        _attitude[0] = (int16_t)(A6_SCALING_1 * magAttitude[0]);
        _attitude[1] = (int16_t)(A6_SCALING_2 * magAttitude[1]);
        _attitude[2] = (int16_t)(A6_SCALING_1 * magAttitude[2]);
        
        _sysTime = (uint32_t) (sysData->systemTime * 1000);
        _bitWord = sysData->bitWord.all;
    }

	XbowDMCDataPacket(uint8_t *bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, MC, 12) {
		_dPtr = bytes;
		ntohb_16((uint16_t*)&_attitude[0]);
		ntohb_16((uint16_t*)&_attitude[1]);
		ntohb_16((uint16_t*)&_attitude[2]);
		ntohb_32((uint32_t*)&_sysTime);
		ntohb_16((uint16_t*)&_bitWord);
	}

    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        for (int i=0; i<3; i++) {
            htonb_16(_attitude[i]);
        }
                     
        htonb_32(_sysTime);
        htonb_16(_bitWord);
    
        return ESUCCESS;
    }
    
};

#endif  // _XBOW_ANGLE_PKT_HPP_

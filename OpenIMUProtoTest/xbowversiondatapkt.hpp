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
* Classes that define the various Xbow version / ID packets
*
* @file    xbowVersionDataPkt.hpp
*/

#ifndef _XBOW_VERSION_DATA_PKT_HPP_
#define _XBOW_VERSION_DATA_PKT_HPP_

#include "xassert.h"
#include "xbowPacket.hpp"
#include "packetDefns.h"


/**
* The ID packet
*/
class XbowIdPacket : public XbowPacket
{
  public:
    uint32_t _serialNumber;
    char _modelString[64];
    int _modelStrLen;
    
    XbowIdPacket(uint32_t serialNumber, char* modelString) : XbowPacket(XBOW_PROTOCOL, ID, 64) {
        if (strlen(modelString) <= 64) {
            _modelStrLen = strlen(modelString);
        }
        else {
            _modelStrLen = 64;
        }
        _payloadLen = 5 + _modelStrLen;  // adjust the payload length to the correct value
        
        _serialNumber = serialNumber;
        strncpy(_modelString, modelString, 64);
    }

    XbowIdPacket(uint8_t* data, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, ID, payloadLen) {
		int i;
		payloadLen -= 5;
        _dPtr      = data;
        ntohb_32(&_serialNumber);
        for (i=0; i< payloadLen; i++) {
            ntohb_8((uint8_t*)&_modelString[i]);
        }
		_modelString[i] = 0;
    }
 
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_32(_serialNumber);
        for (int i=0; i<_modelStrLen; i++) {
            htonb_8(_modelString[i]);
        }
        htonb_8(0x00);  // Send the null character
        
        return ESUCCESS;
    }
    
};

/**
* The Version Data (VR) packet
*/
class XbowSwVersionPacket : public XbowPacket
{
  public:
    uint8_t _softwareVersion[5];
    
    XbowSwVersionPacket(uint8_t* version) : XbowPacket(XBOW_PROTOCOL, VR, 5) {
        
        XASSERT(version != NULL);
        for (int i=0; i<5; i++) {
            _softwareVersion[i] = version[i];
        }
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        for (int i=0; i<5; i++) {
            htonb_8(_softwareVersion[i]);
        }
        return ESUCCESS;
    }
    
};

/**
* The Extended Version Data (VX) packet
*/
class XbowVersionAllPacket : public XbowPacket
{
  public:
    uint8_t _versions[MAX_VERSIONS*5];
    
    XbowVersionAllPacket(uint8_t* versions, int len) : XbowPacket(XBOW_PROTOCOL, VA, MAX_VERSIONS*5) {
        len = (len/5) * 5;
        XASSERT(versions != NULL && len <= MAX_VERSIONS*5);
        
        for (int i=0; i < len; i++) {
            _versions[i] = versions[i];
        }
        _payloadLen = len;
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        for (int i=0; i< _payloadLen; i++) {
            htonb_8(_versions[i]);
        }
        return ESUCCESS;
    }

	XbowVersionAllPacket(uint8_t *bytes, int payloadLen, int dummy) : XbowPacket(XBOW_PROTOCOL, VA, 15) {
		_dPtr = bytes;
        int numVersions = (payloadLen/5)*5;
        if(numVersions > MAX_VERSIONS){
            numVersions = MAX_VERSIONS;
        }
        for (int i = 0; i < payloadLen; i++){
              ntohb_8(&_versions[i]);
        }   
	}
    
    void getVersion(int num, char *buf){
		uint8_t tmp;
		int i;
        
        if(num >= MAX_VERSIONS){
            num = MAX_VERSIONS-1;
        }
        
        for(i = 0; i < 5; i++){
			tmp = _versions[num * 5 + i] | 0x30;
			buf[i * 2] = tmp;
            buf[i*2+1] = '.';
        }
        
        buf[9] = 0;
    }
    
};


#endif  // _XBOW_VERSION_DATA_PKT_HPP_

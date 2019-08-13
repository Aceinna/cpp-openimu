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

#ifndef _XBOW_PACKET_HPP_
#define _XBOW_PACKET_HPP_

#include <stdint.h>
#include <string.h>
#include "ISerializable.hpp"
#include "MemStream.hpp"

// @todo tm20111019 - investigate the real max size we need

// packet format: 0x5555 <pktType U2> <payloadLen U1> <payload> <crc U2>
#define XBOWPKT_MAX_LEN     262  // payload is limited to 2^8-1, plus 2 byte header, 2 byte type, 1 byte len, 2 byte crc
#define XBOW_MSG_SYNC       0x5555

#pragma pack(1)
typedef struct {
    uint16_t    sync;
    uint16_t    type;
    uint8_t     payloadLen;
} xbow_pkt_hdr_t;
#pragma pack()

union FloatConverter_t {
    float fVal;
    uint32_t uVal;
};

/**
 * Defines the generic Xbow packet
 */
class XbowPacket : public ISerializable
{
  protected:
    uint16_t    _protocol;
    uint16_t    _pktType;
    uint8_t     _payloadLen;
    uint8_t     *_dPtr;
    int32_t     *_iPtr;
    uint32_t    *_uPtr;
    
  public:
    using ISerializable::serialize;
    using ISerializable::deserialize;
    uint16_t getPacketType(void) { return _pktType; }
    uint8_t  getPayloadLen(void) { return _payloadLen; }
    uint16_t getProtocol(void) { return _protocol; }
    void     setDataPtr(uint8_t *ptr) { _dPtr = ptr;}
    
    void htonb_float(float f) {
        FloatConverter_t bytes;
        bytes.fVal = f;
        htonb_32(bytes.uVal);
    }

    void ntonh_float(float *f) {
        FloatConverter_t bytes;
        ntohb_32(&bytes.uVal);
        *f = bytes.fVal;
    }

    void htonb_32(uint32_t num){
        *_dPtr++ = b4(num);
        *_dPtr++ = b3(num);
        *_dPtr++ = b2(num);
        *_dPtr++ = b1(num);
    }

	void htoh_32(uint8_t *num) {
		*_dPtr++ = num[0];
		*_dPtr++ = num[1];
		*_dPtr++ = num[2];
		*_dPtr++ = num[3];
	}

	void htoh_64(uint8_t *num) {
		*_dPtr++ = num[0];
		*_dPtr++ = num[1];
		*_dPtr++ = num[2];
		*_dPtr++ = num[3];
		*_dPtr++ = num[4];
		*_dPtr++ = num[5];
		*_dPtr++ = num[6];
		*_dPtr++ = num[7];
	}

	void htoh_16(uint8_t *num) {
		*_dPtr++ = num[0];
		*_dPtr++ = num[1];
	}


    void ntohb_32(uint32_t *num){
        *num  = *_dPtr++;
        *num <<= 8;
        *num |= *_dPtr++;
        *num <<= 8;
        *num |= *_dPtr++;
        *num <<= 8;
        *num |= *_dPtr++;
    }

	void nton_32(uint32_t *num) {
		uint32_t tmp = 0;
		*num = 0;

		tmp   = *_dPtr++;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 8;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 16;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 24;
		*num |= tmp;
	}

	void nton_64(uint64_t *num) {
		uint64_t tmp = 0;
		*num = 0;

		tmp = *_dPtr++;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 8;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 16;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 24;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 32;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 40;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 48;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 56;
		*num |= tmp;
	}

	void nton_16(uint16_t *num) {
		uint32_t tmp = 0;
		*num = 0;

		tmp = *_dPtr++;
		*num |= tmp;

		tmp = *_dPtr++;
		tmp <<= 8;
		*num |= tmp;
	}

	void nton_8(uint8_t *num) {
		*num = *_dPtr++;
	}


	void htonb_16(uint16_t num){
        *_dPtr++ = b2(num);
        *_dPtr++ = b1(num);
    }

    void ntohb_16(uint16_t *num){
        *num = *_dPtr++;
        *num <<= 8;
        *num |= *_dPtr++;
    }

    void htonb_8(uint8_t num){
        *_dPtr++ = num;
    }

    void ntohb_8(uint8_t *num){
        *num = *_dPtr++;
    }

    virtual ~XbowPacket() {}
    XbowPacket() {}
    XbowPacket(uint16_t protocol, uint16_t pktType, uint8_t len);

    virtual error_t prepareData() {
        return -EFAIL;
    }
    
    virtual error_t send(IDataStream *stream);
};


// -------------------------- INLINE METHODS -----------------------------------

/// Constructor for Xbow packet header
inline XbowPacket::XbowPacket(uint16_t protocol, uint16_t pktType, uint8_t len) {
    _protocol   = protocol;
    _pktType    = pktType;
    _payloadLen = len;
}


#endif  // _XBOW_PACKET_HPP_

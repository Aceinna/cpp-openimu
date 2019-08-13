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
* Classes that define the various Xbow command and response packets
*
* @file    xbowCmdResponsePkt.hpp
*/

#ifndef _OPEN_IMU_CMD_RESPONSE_PKT_HPP_
#define _OPEN_IMU_CMD_RESPONSE_PKT_HPP_

#include "xassert.h"

#include "xbowPacket.hpp"
#include "packetDefns.h"
#include "StreamBuffer.hpp"

#ifndef MAX_PARAMS
#define MAX_PARAMS 30
#endif


/**
* Response to a ping, PK
*/
class OpenIMUPingPacket : public XbowPacket
{
  public:
    
    ~OpenIMUPingPacket() {}
    OpenIMUPingPacket(void) : XbowPacket(XBOW_PROTOCOL, USR_IN_PING, 0) {}
    
};

class OpenIMUVersionPacket : public XbowPacket
{
public:

	~OpenIMUVersionPacket() {}
	OpenIMUVersionPacket(void) : XbowPacket(XBOW_PROTOCOL, USR_IN_GET_VERSION, 0) {}

};



/**
* Request to an error, NAK response
*/
class OpenIMUNAKPacket : public XbowPacket
{
  public:
    using ISerializable::serialize;
    uint16_t _pktErrMsg;

    ~OpenIMUNAKPacket() {}
    OpenIMUNAKPacket(uint16_t pktErrCode) : XbowPacket(XBOW_PROTOCOL, NAK, 2) {
        _pktErrMsg = pktErrCode;        //get 2 bytes error message from passed argument
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_16(_pktErrMsg);           //add the error to string buffer for serial port send out

        return ESUCCESS;
    }
    
};

/**
* Request to Software Reset command, SR command
*/

class OpenIMUResetCmdPacket : public XbowPacket
{
  public:
    
    ~OpenIMUResetCmdPacket() {}

	OpenIMUResetCmdPacket() : XbowPacket(XBOW_PROTOCOL, USR_IN_RESET, 0) {
	}

	virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter = NULL) {
		return ESUCCESS;        // no need to write anything to buffer since payload is 0
	}

};

/**
* Request to Software Reset response, SR response
*/

class OpenIMUResetResponsePacket : public XbowPacket
{
  public:
    

	  OpenIMUResetResponsePacket() : XbowPacket(XBOW_PROTOCOL, USR_IN_RESET, 0) {};
	  OpenIMUResetResponsePacket(uint16_t numFields) : XbowPacket(XBOW_PROTOCOL, USR_IN_RESET, 0) {};
	
	~OpenIMUResetResponsePacket() {}

    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        return ESUCCESS;        // no need to write anything to buffer since payload is 0
    }

};


/**
* User request for ID, VR, S0, A6, T3 packet, GP command
*/
class OpenIMUGetPacket : public XbowPacket
{
  public:
    using ISerializable::serialize;
    using ISerializable::deserialize;
    uint16_t _pktRequested;  // for GP command

    ~OpenIMUGetPacket() {}
    OpenIMUGetPacket(uint8_t* bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, GP, 0) {
        StreamBuffer<STREAM_BUFFER_SIZE> stream;
        stream.write(bytes, payloadLen);        // payload length is from serial port read in TaskMessaging.cpp
        deserialize(&stream);
    }

	OpenIMUGetPacket(uint16_t pktType) : XbowPacket(XBOW_PROTOCOL, GP, 2) {
		_pktRequested = pktType;
	}

    virtual error_t deserialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        _pktRequested = stream->readShort();    // read the packet type from stream and
        return ESUCCESS;
    }

	virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter = NULL) {
		htonb_16(_pktRequested);           //add the error to string buffer for serial port send out
		return ESUCCESS;
	}

};

class OpenIMUWACmdPacket : public XbowPacket
{
public:
	using ISerializable::serialize;
	using ISerializable::deserialize;

	uint32_t _numFields;
	uint32_t _offset;
	uint64_t _param;
	uint8_t  bbb = 4;

	OpenIMUWACmdPacket(uint32_t offset, uint32_t value) : XbowPacket(XBOW_PROTOCOL, WA, 1) {
		_offset      = offset;
		_payloadLen = 9;
		_param      = value;
	}

	virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter = NULL) {
		htoh_32((uint8_t*)&_offset);
		htonb_8(bbb);
		htoh_32((uint8_t*)&_param);

		return ESUCCESS;
	}

};


class OpenIMUSetParamCmdPacket : public XbowPacket
{
public:
	using ISerializable::serialize;
	using ISerializable::deserialize;

	uint32_t _numFields;
	uint32_t _offset;
	uint64_t _param;

	OpenIMUSetParamCmdPacket(uint32_t offset, uint64_t value) : XbowPacket(XBOW_PROTOCOL, USR_IN_UPDATE_PARAM, 1) {
		_offset = offset;
		_payloadLen = 12;
		_param = value;
	}

	virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter = NULL) {
		htoh_32((uint8_t*)&_offset);
		htoh_64((uint8_t*)&_param);

		return ESUCCESS;
	}

};

class OpenIMUSetParamResponsePacket : public XbowPacket
{
public:
	int32_t _numFields;

	OpenIMUSetParamResponsePacket(uint8_t* data) : XbowPacket(XBOW_PROTOCOL, USR_IN_UPDATE_PARAM, 1) {
		_dPtr = data;
		nton_32((uint32_t*)&_numFields);
	}
};



/**
* User request to set field data, SF command
*/
class OpenIMUSetParamsCmdPacket : public XbowPacket
{
public:
    using ISerializable::serialize;
    using ISerializable::deserialize;
    
    uint32_t _numFields;  
	uint32_t _offset;     
	uint64_t _params[MAX_PARAMS];

    OpenIMUSetParamsCmdPacket(uint16_t numParams, uint16_t offset, uint64_t *values) : XbowPacket(XBOW_PROTOCOL, USR_IN_UPDATE_CONFIG, 1) {
        XASSERT(numParams <= MAX_PARAMS);
        _numFields  = numParams;
		_offset     = offset;
		_payloadLen = 8;
		for(int i = 0; i < numParams; i++){
            _params[i]   = values[i];
            _payloadLen += 8;
        }
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htoh_32((uint8_t*)&_numFields);
		htoh_32((uint8_t*)&_offset);
		for (int i=0; i<_numFields; i++) {
            htoh_64((uint8_t*)&_params[i]);
        }
    
        return ESUCCESS;
    }
    
};

/**
* User request to set field data, SF command
*/
class OpenIMUSetAllCmdPacket : public XbowPacket
{
public:
	using ISerializable::serialize;
	using ISerializable::deserialize;

	uint32_t _numFields;
	uint64_t _params[MAX_PARAMS];

	OpenIMUSetAllCmdPacket(uint32_t numParams, uint64_t *values) : XbowPacket(XBOW_PROTOCOL, USR_IN_UPDATE_ALL, 0) {
		XASSERT(numParams <= MAX_PARAMS);
		_numFields = numParams;
		for (int i = 0; i < numParams; i++) {
			_params[i] = values[i];
			_payloadLen += 8;
		}
	}

	virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter = NULL) {
		for (int i = 0; i<_numFields; i++) {
			htoh_64((uint8_t*)&_params[i]);
		}

		return ESUCCESS;
	}

};

/**
* Response to user request to set field data, SF response
*/
class OpenIMUSetAllResponsePacket : public XbowPacket
{
public:
	uint32_t _len;
	int32_t  _numFields;

	OpenIMUSetAllResponsePacket(uint8_t * data, int len) : XbowPacket(XBOW_PROTOCOL, USR_IN_UPDATE_ALL, 1) {
		_dPtr = data;
		nton_32((uint32_t*)&_numFields);
	}
};



/**
* Response to user request to set field data, SF response
*/
class OpenIMUSetParamsResponsePacket : public XbowPacket
{
  public:
    int32_t  _numFields;
	uint32_t _offset;
    uint64_t _params[MAX_PARAMS];
    
    OpenIMUSetParamsResponsePacket(uint8_t* data) : XbowPacket(XBOW_PROTOCOL, USR_IN_UPDATE_CONFIG, 1) {
       _dPtr      = data;
        nton_32((uint32_t*)&_numFields);
		nton_32(&_offset);
		for (int i=0; i<_numFields && i < MAX_PARAMS; i++) {
             nton_64(&_params[i]);
        }
    }
};

/**
* User request to set field data, SF command
*/
class OpenIMUGetParamsCmdPacket : public XbowPacket
{
public:
	using ISerializable::serialize;
	using ISerializable::deserialize;

	uint32_t _numFields;
	uint32_t _offset;
	uint64_t _params[MAX_PARAMS];

	OpenIMUGetParamsCmdPacket(uint32_t numParams, uint32_t offset) : XbowPacket(XBOW_PROTOCOL, USR_IN_GET_CONFIG, 1) {
		XASSERT(numParams <= MAX_PARAMS);
		_numFields = numParams;
		_offset = offset;
		_payloadLen = 8;
	}

	virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter = NULL) {
		htoh_32((uint8_t*)&_numFields);
		htoh_32((uint8_t*)&_offset);
		return ESUCCESS;
	}

};



/**
* Response to user request to set field data, SF response
*/
class OpenIMUGetParamsResponsePacket : public XbowPacket
{
public:
	uint32_t _numFields;
	uint32_t _offset;
	uint64_t _params[MAX_PARAMS];

	OpenIMUGetParamsResponsePacket(uint8_t* data) : XbowPacket(XBOW_PROTOCOL, USR_IN_GET_CONFIG, 1) {
		_dPtr = data;
		nton_32(&_numFields);
		nton_32(&_offset);
		for (int i = 0; i<_numFields && i < MAX_PARAMS; i++) {
			nton_64(&_params[i]);
		}
	}
};


/**
* User request to set field data, SF command
*/
class OpenIMUGetParamCmdPacket : public XbowPacket
{
public:
	using ISerializable::serialize;
	using ISerializable::deserialize;

	uint32_t _offset;
	uint64_t _param;

	OpenIMUGetParamCmdPacket(uint32_t offset) : XbowPacket(XBOW_PROTOCOL, USR_IN_GET_PARAM, 1) {
		_offset     = offset;
		_payloadLen = 4;
	}

	virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter = NULL) {
		htoh_32((uint8_t*)&_offset);
		return ESUCCESS;
	}

};



/**
* Response to user request to set field data, SF response
*/
class OpenIMUGetParamResponsePacket : public XbowPacket
{
public:
	uint32_t _offset;
	uint64_t _param;

	OpenIMUGetParamResponsePacket(uint8_t* data) : XbowPacket(XBOW_PROTOCOL, USR_IN_GET_PARAM, 1) {
		_dPtr = data;
		nton_32(&_offset);
		nton_64(&_param);
	}
};


/**
* User request to set field data, SF command
*/
class OpenIMUGetAllCmdPacket : public XbowPacket
{
public:
	using ISerializable::serialize;
	using ISerializable::deserialize;

	uint32_t _numFields;
	uint32_t _offset;

	OpenIMUGetAllCmdPacket() : XbowPacket(XBOW_PROTOCOL, USR_IN_GET_ALL, 0) {
		_payloadLen = 0;
	}

	virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter = NULL) {
		return ESUCCESS;
	}

};



/**
* Response to user request to set field data, SF response
*/
class OpenIMUGetAllResponsePacket : public XbowPacket
{
public:
	uint64_t _params[MAX_PARAMS];
	int      _len;
	OpenIMUGetAllResponsePacket(uint8_t* data, int size) : XbowPacket(XBOW_PROTOCOL, USR_IN_GET_ALL, 1) {
		_len = size / 8;
		XASSERT(_len <= MAX_PARAMS);
		_dPtr = data;
		for (int i = 0; i< _len ; i++) {
			nton_64(&_params[i]);
		}
	}
};



/**
* User request to write EEPROM
*/
class OpenIMUSaveConfigCmdPacket : public XbowPacket
{
public:
    using ISerializable::serialize;
    using ISerializable::deserialize;

    uint8_t   _numWords;             
    uint16_t  _startWordIdx;             
    uint16_t  _data[MAX_FIELDS]; 

	OpenIMUSaveConfigCmdPacket() : XbowPacket(XBOW_PROTOCOL, USR_IN_SAVE_CONFIG, 0) {
    }

};

/**
* Response to user request to write EEPROM(s) data
*/
class OpenIMUSaveConfigResponsePacket : public XbowPacket
{
  public:
 
    uint16_t  _startWordIdx;  
    uint8_t   _numWords;   // less than [MAX_FIELDS-2]
    
    ~OpenIMUSaveConfigResponsePacket() {}
	OpenIMUSaveConfigResponsePacket(uint16_t pktType, uint16_t startWordIdx, uint8_t numWords) :
              XbowPacket(XBOW_PROTOCOL, pktType, 3) {
        _startWordIdx = startWordIdx;
        _numWords = numWords;
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_16(_startWordIdx);
        htonb_8(_numWords);
        return ESUCCESS;
    }
    
};


/**
* User request to write EEPROM
*/
class OpenIMURestoreDefaultsCmdPacket : public XbowPacket
{
public:
	using ISerializable::serialize;
	using ISerializable::deserialize;

	uint8_t   _numWords;
	uint16_t  _startWordIdx;
	uint16_t  _data[MAX_FIELDS];

	OpenIMURestoreDefaultsCmdPacket() : XbowPacket(XBOW_PROTOCOL, USR_IN_RESTORE_DEFAULTS, 0) {
	}

};

/**
* User request to lock EEPROM
*/
class OpenIMULockEEpromCmdPacket : public XbowPacket
{
public:
	using ISerializable::serialize;
	using ISerializable::deserialize;

	uint8_t   _numWords;
	uint16_t  _startWordIdx;
	uint16_t  _data[MAX_FIELDS];

	OpenIMULockEEpromCmdPacket() : XbowPacket(XBOW_PROTOCOL, LE, 0) {
	}

};

/**
* User request to lock EEPROM
*/
class OpenIMUUnlockEEpromCmdPacket : public XbowPacket
{
public:
	using ISerializable::serialize;
	using ISerializable::deserialize;

	uint8_t   _numWords;
	uint16_t  _startWordIdx;
	uint16_t  _data[MAX_FIELDS];

	OpenIMUUnlockEEpromCmdPacket() : XbowPacket(XBOW_PROTOCOL, UE, 0) {
	}

};





/**
* Scaled sensor 0 message
*/
class OpenIMUData1Pkt : public XbowPacket
{
public:
	int i = 0;
	uint32_t _tStamp;
	float arr[10];
	float  *_ptr;
	float  _accel[3];
	float  _rate[3];
	float  _mag[3];

	OpenIMUData1Pkt(uint8_t *bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, USR_OUT_DATA1, 46) {
		_dPtr = bytes;
		memcpy(arr, bytes+4, 36);
		_ptr    = arr;
		nton_32(&_tStamp);
		for (; i<3; i++) {
			_accel[i]  = _ptr[i];
			_rate[i]   = _ptr[i+3];
			_mag[i]    = _ptr[i+6];
		}

	}

};


class OpenIMUData2Pkt : public XbowPacket
{
public:
	int i = 0;
	uint32_t _tStamp;
	uint8_t  _byte;
	uint16_t _short;
	int      _int;
	uint64_t _ll;
	double   _dbl;

	OpenIMUData2Pkt(uint8_t *bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, USR_OUT_DATA1, 46) {
		_dPtr = bytes;
		nton_32(&_tStamp);
		nton_8(&_byte);
		nton_16(&_short);
		nton_32((uint32_t*)&_int);
		nton_64((uint64_t*)&_ll);
		nton_64((uint64_t*)&_dbl);
	}

};



#endif  // _XBOW_CMD_RESPONSE_PKT_HPP_

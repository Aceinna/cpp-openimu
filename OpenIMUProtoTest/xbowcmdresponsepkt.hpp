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

#ifndef _XBOW_CMD_RESPONSE_PKT_HPP_
#define _XBOW_CMD_RESPONSE_PKT_HPP_

#include "xassert.h"

#include "xbowPacket.hpp"
#include "packetDefns.h"
#include "StreamBuffer.hpp"

#ifndef MAX_FIELDS
#define MAX_FIELDS 128  // @The max packet payload is 256 bytes->128 words
#endif


/**
* Response to a ping, PK
*/
class XbowPingPacket : public XbowPacket
{
  public:
    
    ~XbowPingPacket() {}
    XbowPingPacket(void) : XbowPacket(XBOW_PROTOCOL, PK, 0) {}
    
};

/**
* Response to a ping, PK
*/
class XbowJIPacket : public XbowPacket
{
public:

	~XbowJIPacket() {}
	XbowJIPacket(void) : XbowPacket(XBOW_PROTOCOL, JI, 0) {}

};

/**
* Response to a ping, PK
*/
class XbowJAPacket : public XbowPacket
{
public:

	~XbowJAPacket() {}
	XbowJAPacket(void) : XbowPacket(XBOW_PROTOCOL, JA, 0) {}

};



/**
* Request to an error, NAK response
*/
class XbowNAKPacket : public XbowPacket
{
  public:
    using ISerializable::serialize;
    uint16_t _pktErrMsg;

    ~XbowNAKPacket() {}
    XbowNAKPacket(uint16_t pktErrCode) : XbowPacket(XBOW_PROTOCOL, NAK, 2) {
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

class XbowSoftwareResetCmdPacket : public XbowPacket
{
  public:
    
    ~XbowSoftwareResetCmdPacket() {}
    XbowSoftwareResetCmdPacket(uint8_t* bytes) : XbowPacket(XBOW_PROTOCOL, SR, 0) {
        StreamBuffer<STREAM_BUFFER_SIZE> stream; // prepare 'stream' to get serial port data
        stream.write(bytes, 0);                  // write payload length to buffer
    }

	XbowSoftwareResetCmdPacket() : XbowPacket(XBOW_PROTOCOL, SR, 0) {
	}

	virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter = NULL) {
		return ESUCCESS;        // no need to write anything to buffer since payload is 0
	}

};

/**
* Request to Software Reset response, SR response
*/

class XbowSoftwareResetResponsePacket : public XbowPacket
{
  public:
    

    XbowSoftwareResetResponsePacket() : XbowPacket(XBOW_PROTOCOL, SR, 0) {};
	XbowSoftwareResetResponsePacket(uint16_t numFields) : XbowPacket(XBOW_PROTOCOL, SR, 0) {};
	
	~XbowSoftwareResetResponsePacket() {}

    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        return ESUCCESS;        // no need to write anything to buffer since payload is 0
    }

};


/**
* User request for ID, VR, S0, A6, T3 packet, GP command
*/
class XbowGetPacket : public XbowPacket
{
  public:
    using ISerializable::serialize;
    using ISerializable::deserialize;
    uint16_t _pktRequested;  // for GP command

    ~XbowGetPacket() {}
    XbowGetPacket(uint8_t* bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, GP, 0) {
        StreamBuffer<STREAM_BUFFER_SIZE> stream;
        stream.write(bytes, payloadLen);        // payload length is from serial port read in TaskMessaging.cpp
        deserialize(&stream);
    }

	XbowGetPacket(uint16_t pktType) : XbowPacket(XBOW_PROTOCOL, GP, 2) {
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


/**
* User request for get field data, GF command
*/
class XbowGetFieldsCmdPacket : public XbowPacket
{
public:
    using ISerializable::serialize;
    using ISerializable::deserialize;
    uint8_t _numFields;  // for GF and SF commands
    uint16_t _fieldIds[MAX_FIELDS];  // for GF and SF commands

    ~XbowGetFieldsCmdPacket() {}
    XbowGetFieldsCmdPacket(uint8_t* bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, GF, payloadLen) {
        XASSERT(bytes != NULL);
        StreamBuffer<STREAM_BUFFER_SIZE> stream;
        stream.write(bytes, payloadLen);        // write payload length to stream first
        deserialize(&stream);
    }

	XbowGetFieldsCmdPacket(int numFields, uint16_t *fieldIds) : XbowPacket(XBOW_PROTOCOL, GF, 1) {
		_numFields = numFields;        // read 1-byte data length
		for (int i = 0; i<_numFields; i++) {     
			_fieldIds[i] = fieldIds[i];			 
			_payloadLen += 2;
		}
	}

	virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter = NULL) {
		htonb_8(_numFields);
		for (int i = 0; i<_numFields; i++) {
			htonb_16(_fieldIds[i]);
		}
		return ESUCCESS;
	}


    virtual error_t deserialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        _numFields = stream->readByte();        // read 1-byte data length
        for (int i=0; i<_numFields; i++) {      // read all data based on the 1 byte length
            _fieldIds[i] = stream->readShort(); // and save them to packet
        }
        
        return ESUCCESS;
    }
    
};

/**
* Response to user request for field data, GF response
*/
class XbowGetFieldsResponsePacket : public XbowPacket
{
  public:
    uint8_t _numFields;
    uint16_t _fieldIds[MAX_FIELDS];
    uint16_t _fieldData[MAX_FIELDS];
    
    ~XbowGetFieldsResponsePacket() {}
    XbowGetFieldsResponsePacket(uint16_t numFields, uint16_t *fieldIds, uint16_t *fieldData) : 
      XbowPacket(XBOW_PROTOCOL, GF, 1 + 4*numFields) {
        XASSERT( (fieldIds != NULL) && (fieldData != NULL) && (numFields <= MAX_FIELDS) );
        _numFields = numFields;                 // read data length from message queue
        for (int i=0; i<numFields; i++) {       // save data from queue to local members
            _fieldIds[i] = fieldIds[i];
            _fieldData[i] = fieldData[i];
        }
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_8(_numFields);                    // save data length to buffer
        for (int i=0; i<_numFields; i++) {      // save all IDs and data to buffer
            htonb_16(_fieldIds[i]);             // for sending to serial port
            htonb_16(_fieldData[i]);
        }
        
        return ESUCCESS;
    }

	XbowGetFieldsResponsePacket(uint8_t *data, int len) : XbowPacket(XBOW_PROTOCOL, GF, 1) {
		_dPtr = data;
		ntohb_8(&_numFields);                    // save data length to buffer
		for (int i = 0; i<_numFields; i++) {      // save all IDs and data to buffer
			ntohb_16(&_fieldIds[i]);             // for sending to serial port
			ntohb_16(&_fieldData[i]);
		}
	}


};


/**
* User request for field data from EEPROM (flash), RF command
*/
class XbowReadFieldsCmdPacket : public XbowPacket
{
public:
    using ISerializable::serialize;
    using ISerializable::deserialize;
    
    uint8_t  _numFields;             
    uint16_t _fieldIds[MAX_FIELDS];  

    ~XbowReadFieldsCmdPacket() {}
    XbowReadFieldsCmdPacket(uint8_t* bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, RF, payloadLen) {
        XASSERT(bytes != NULL);
        StreamBuffer<STREAM_BUFFER_SIZE> stream;
        stream.write(bytes, payloadLen);        // write payload length to stream first
        deserialize(bytes, payloadLen);
    }
    
    //virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
    //    htonb_8(_numFields);
    //    for (int i=0; i<_numFields; i++) {
    //        htonb_16(_fieldIds[i]);
    //    }

    //    return ESUCCESS;
    //}
    
    virtual error_t deserialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        _numFields = stream->readByte();        // read 1-byte data length
        for (int i=0; i<_numFields; i++) {      // read all data based on the 1 byte length
            _fieldIds[i] = stream->readShort(); // and save them to packet
        }
        
        return ESUCCESS;
    }


    virtual error_t deserialize(uint8_t *data, int len) {
       _dPtr      = data;
        ntohb_8(&_numFields );
        for (int i=0; i<_numFields; i++) {
             ntohb_16(&_fieldIds[i]);
        }
        
        return ESUCCESS;
    } 
    
};


/**
* Response to user request for field data, RF response
*/
class XbowReadFieldsResponsePacket : public XbowPacket
{
  public:
    uint8_t _numFields;
    uint16_t _fieldIds[MAX_FIELDS/2];
    uint16_t _fieldData[MAX_FIELDS/2];
    
    ~XbowReadFieldsResponsePacket() {}
    XbowReadFieldsResponsePacket(uint16_t numFields, uint16_t *fieldIds, uint16_t *fieldData) : 
      XbowPacket(XBOW_PROTOCOL, RF, 1 + 4*numFields) {
        XASSERT( (fieldIds != NULL) && (fieldData != NULL) && (numFields <= MAX_FIELDS/2));
        _numFields = numFields;                 // read data length from message queue
        for (int i=0; i<numFields; i++) {       // save data from queue to local members
            _fieldIds[i] = fieldIds[i];
            _fieldData[i] = fieldData[i];
        }
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_8(_numFields);                    // save data length to buffer
        for (int i=0; i<_numFields; i++) {      // save all IDs and data to buffer
            htonb_16(_fieldIds[i]);             // for sending to serial port
            htonb_16(_fieldData[i]);
        }
        
        return ESUCCESS;
    }
    
};


/**
* User request to set field data, SF command
*/
class XbowSetFieldsCmdPacket : public XbowPacket
{
public:
    using ISerializable::serialize;
    using ISerializable::deserialize;
    
    uint8_t _numFields;  // for GF and SF commands
    uint16_t _fieldIds[MAX_FIELDS/2];  // for GF and SF commands
    uint16_t _fieldData[MAX_FIELDS/2];  // for SF commands

    XbowSetFieldsCmdPacket(uint8_t* bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, SF, payloadLen) {
        XASSERT(bytes != NULL);
        StreamBuffer<STREAM_BUFFER_SIZE> stream;
        stream.write(bytes, payloadLen);        // write payload length to stream first
        deserialize(&stream);
    }

    XbowSetFieldsCmdPacket(uint8_t numFields, uint16_t *fieldIds, uint16_t *fieldData) : XbowPacket(XBOW_PROTOCOL, SF, 1) {
        XASSERT(numFields <= MAX_FIELDS/2);
        _numFields = numFields;
        for(int i = 0; i < numFields; i++){
            _fieldIds[i]  = *fieldIds++;
            _fieldData[i] = *fieldData++;
            _payloadLen += 4;
        }
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_8(_numFields);
        for (int i=0; i<_numFields; i++) {
            htonb_16(_fieldIds[i]);
            htonb_16(_fieldData[i]);
        }
    
        return ESUCCESS;
    }
    
    virtual error_t deserialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        _numFields = stream->readByte();        // read 1-byte data length
        XASSERT(_numFields <= MAX_FIELDS/2);
        for (int i=0; i<_numFields; i++) {
            _fieldIds[i] = stream->readShort(); // read all data based on the 1 byte length
            _fieldData[i] = stream->readShort();// and save them to packet
        }
        
        return ESUCCESS;
    } 
};

/**
* Response to user request to set field data, SF response
*/
class XbowSetFieldsResponsePacket : public XbowPacket
{
  public:
    uint8_t _numFields;
    uint16_t _fieldIds[MAX_FIELDS];
    
    XbowSetFieldsResponsePacket() {}
    XbowSetFieldsResponsePacket(uint16_t numFields, uint16_t *fieldIds) : 
    XbowPacket(XBOW_PROTOCOL, SF, 1 + 2*numFields) {
        XASSERT(fieldIds != NULL);
        _numFields = numFields;                 // read data length from message queue
        for (int i=0; i<numFields; i++) {       // save data from queue to local members
            _fieldIds[i] = fieldIds[i];
        }
    }

    XbowSetFieldsResponsePacket(uint8_t* data) : XbowPacket(XBOW_PROTOCOL, SF, 1) {
       _dPtr      = data;
        ntohb_8(&_numFields);
        for (int i=0; i<_numFields && i < MAX_FIELDS; i++) {
             ntohb_16(&_fieldIds[i]);
        }
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_8(_numFields);                    // save data length to buffer
        for (int i=0; i<_numFields; i++) {      // save all IDs to buffer
            htonb_16(_fieldIds[i]);             // for sending to serial port
        }
        
        return ESUCCESS;
    }
    
};


/**
* User request to write field(s) data, WF command
*/
class XbowWriteFieldsCmdPacket : public XbowPacket
{
public:
    using ISerializable::serialize;
    using ISerializable::deserialize;

    uint8_t  _numFields;             
    uint16_t _fieldIds[MAX_FIELDS/2];  
    uint16_t _fieldData[MAX_FIELDS/2]; 

    XbowWriteFieldsCmdPacket(uint8_t* bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, WF, payloadLen) {
        XASSERT(bytes != NULL);
        deserialize(bytes, payloadLen);
    }
    
    //virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
    //    htonb_8(_numFields);
    //    for (int i=0; i<_numFields; i++) {
    //        htonb_16(_fieldIds[i]);
    //    }
    //
    //    return ESUCCESS;
    //}
    
    virtual error_t deserialize(uint8_t *data, int len) {
       _dPtr      = data;
        ntohb_8(&_numFields );
        XASSERT(_numFields <= MAX_FIELDS/2)
        for (int i=0; i<_numFields; i++) {
             ntohb_16(&_fieldIds[i]);
             ntohb_16(&_fieldData[i]);
        }
        
        return ESUCCESS;
    } 
};


/**
* Response to user request to write field(s) data, WF response
*/
class XbowWriteFieldsResponsePacket : public XbowPacket
{
  public:
    uint8_t _numFields;
    uint16_t _fieldIds[MAX_FIELDS];
    
    XbowWriteFieldsResponsePacket() {}
    XbowWriteFieldsResponsePacket(uint16_t numFields, uint16_t *fieldIds) : 
    XbowPacket(XBOW_PROTOCOL, WF, 1 + 2*numFields) {
        XASSERT(fieldIds != NULL);
        _numFields = numFields;
        for (int i=0; i<numFields; i++) {
            _fieldIds[i] = fieldIds[i];
        }
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_8(_numFields);
        for (int i=0; i<_numFields; i++) {
            htonb_16(_fieldIds[i]);
        }
        
        return ESUCCESS;
    }
    
};


/**
* User request to write EEPROM
*/
class XbowWESECmdPacket : public XbowPacket
{
public:
    using ISerializable::serialize;
    using ISerializable::deserialize;

    uint8_t   _numWords;             
    uint16_t  _startWordIdx;             
    uint16_t  _data[MAX_FIELDS]; 

    XbowWESECmdPacket(int type, uint8_t* bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, type, payloadLen) {
        XASSERT(bytes != NULL);
        deserialize(bytes, payloadLen);
    }

    XbowWESECmdPacket(int type, int startWordIdx, uint16_t* data, int numWords) : XbowPacket(XBOW_PROTOCOL, type, numWords*2+3) {
		XASSERT(numWords < MAX_FIELDS);
		_startWordIdx = startWordIdx;
		_numWords = numWords;
		for(int i = 0; i < numWords; i++){
			_data[i] = data[i];
		}
    }
   
    virtual error_t deserialize(uint8_t *data, int len) {
       _dPtr      = data;
        ntohb_16(&_startWordIdx);
        ntohb_8(&_numWords );
        for (int i=0; i<_numWords; i++) {
             ntohb_16(&_data[i]);
        }       
        return ESUCCESS;
    } 

    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_16(_startWordIdx);
        htonb_8(_numWords);
        for (int i=0; i<_numWords; i++) {
             htonb_16(_data[i]);
        }       
        return ESUCCESS;
    } 

};

/**
* Response to user request to write EEPROM(s) data
*/
class XbowWESEResponsePacket : public XbowPacket
{
  public:
 
    uint16_t  _startWordIdx;  
    uint8_t   _numWords;   // less than [MAX_FIELDS-2]
    
    ~XbowWESEResponsePacket() {}
    XbowWESEResponsePacket(uint16_t pktType, uint16_t startWordIdx, uint8_t numWords) :
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
* User request to read from EEPROM
*/
class XbowReadEEPromCmdPacket : public XbowPacket
{
public:
    using ISerializable::serialize;
    using ISerializable::deserialize;           
    uint16_t  _startWordIdx;  
    uint8_t   _numWords;   // less than [MAX_FIELDS-2]
    uint16_t  _data[MAX_FIELDS]; 

    XbowReadEEPromCmdPacket(uint8_t* bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, RE, payloadLen) {
        XASSERT(bytes != NULL);
        deserialize(bytes, payloadLen);
    }
   
    
    virtual error_t deserialize(uint8_t *data, int len) {
       _dPtr      = data;
        ntohb_16(&_startWordIdx);
        ntohb_8(&_numWords );
        return ESUCCESS;
    } 
};


/**
* Response to user request for data from EEPROM (flash)
*/
class XbowReadEEPromResponsePacket : public XbowPacket
{
  public:
    uint16_t _startWordIdx;
    uint8_t  _wordLength;      // less than [MAX_FIELDS-3]
    uint16_t  _data[MAX_FIELDS]; 
    
    ~XbowReadEEPromResponsePacket() {}
    XbowReadEEPromResponsePacket(uint16_t startWordIdx, uint8_t wordLength, uint16_t *data) : 
      XbowPacket(XBOW_PROTOCOL, RE, 3 + 2*wordLength) {
        XASSERT(data != NULL);
        _startWordIdx = startWordIdx;
        _wordLength = wordLength;
        for (int i=0; i<wordLength; i++) {
            _data[i] = data[i];
        }
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_16(_startWordIdx);
        htonb_8(_wordLength);
        for (int i=0; i<_wordLength; i++) {
            htonb_16(_data[i]);
        }  
        return ESUCCESS;
    }
    
};


/**
* User request to unlock the EEPROM
*/
class XbowUnlockEEPromCmdPacket : public XbowPacket
{
 public:
    using ISerializable::serialize;
    using ISerializable::deserialize;
    uint16_t _unlockCRC;  // for a key to unlock the EEPROM

    ~XbowUnlockEEPromCmdPacket() {}
    XbowUnlockEEPromCmdPacket(uint8_t* bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, UE, 0) {
        StreamBuffer<STREAM_BUFFER_SIZE> stream;
        stream.write(bytes, payloadLen);
        deserialize(&stream);
    }

    XbowUnlockEEPromCmdPacket(uint16_t crc) : XbowPacket(XBOW_PROTOCOL, UE, 2) {
    	_unlockCRC = crc;
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_16(_unlockCRC);     
        return ESUCCESS;
    }
    
    virtual error_t deserialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        _unlockCRC = stream->readShort();
        return ESUCCESS;
    }
};

class XbowUnlockEEPromRspPacket : public XbowPacket
{
  public:
    
    ~XbowUnlockEEPromRspPacket() {}
    XbowUnlockEEPromRspPacket(void) : XbowPacket(XBOW_PROTOCOL, UE, 0) {}   
};


/**
* error response packet
*/
class XbowErrorResponsePacket : public XbowPacket
{
  public:
    uint16_t _cmd; 

    ~XbowErrorResponsePacket() {}
    XbowErrorResponsePacket(uint16_t cmd) : XbowPacket(XBOW_PROTOCOL, 0x1515, 2), _cmd(cmd) {};
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_16(_cmd);
        return ESUCCESS;
    }
    
};




#endif  // _XBOW_CMD_RESPONSE_PKT_HPP_

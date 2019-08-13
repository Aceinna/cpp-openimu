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
* Classes that define the various Xbow Factory scaled sensor data packets
*
* @file    xbowSensorScaledPkt.hpp
*/

#ifndef _XBOW_SENSOR_SCALED_PKT_HPP_
#define _XBOW_SENSOR_SCALED_PKT_HPP_

#include "xbowPacket.hpp"
#include "packetDefns.h"
#include "nfDataTypes.hpp"
#include "mathConstants.h"

#define ACCEL_SCALING       3276.8  // MAX_INT16_OVER_20
#define GYRO_SCALING        2980.10810013  // MAX_INT16_OVER_7PI
#define TEMP_SCALING        327.68  // MAX_INT16_OVER_200
#define MAG_SCALING         32768  // 2 / 2^16
#define S4_TEMP_SCALING     100.0


/**
* Scaled sensor 0 message
*/
class XbowSensorScaled0Pkt : public XbowPacket
{
  public:
    int16_t _accel[3];
    int16_t _rate[3];
    int16_t _mag[3];
    int16_t _rateTemp[3];
    int16_t _boardTemp;
    uint16_t _gpsItow;
    uint16_t _bitStatus;
    
	XbowSensorScaled0Pkt(uint8_t *bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, S0, 46) {
		_dPtr = bytes;

		for (int i = 0; i<3; i++) {
			ntohb_16((uint16_t *)&_accel[i]);
		}

		for (int i = 0; i<3; i++) {
			ntohb_16((uint16_t *)&_rate[i]);
		}


		for (int i = 0; i<3; i++) {
			ntohb_16((uint16_t *)&_mag[i]);
		}

		for (int i = 0; i<3; i++) {
			ntohb_16((uint16_t *)&_rateTemp[i]);
		}

		ntohb_16((uint16_t *)&_boardTemp);
		ntohb_16(&_gpsItow);
		ntohb_16(&_bitStatus);
	}
    
};


/**
* Scaled sensor M message
*/
class XbowSensorScaledMPkt : public XbowPacket
{
public:
	int16_t _accel[4][3];
	int16_t _rate[4][3];
	int16_t _rateTemp[4];

	XbowSensorScaledMPkt(uint8_t *bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, SM, 46) {
		_dPtr = bytes;

		for (int n = 0; n < 4; n++) {

			for (int i = 0; i < 3; i++) {
				ntohb_16((uint16_t *)&_accel[n][i]);
			}

			for (int i = 0; i < 3; i++) {
				ntohb_16((uint16_t *)&_rate[n][i]);
			}
			ntohb_16((uint16_t *)&_rateTemp[n]);
		}
	}
};


/**
* Scaled sensor 1 message
*/
class XbowSensorScaled1Pkt : public XbowPacket
{
  public:
    int16_t _accel[3];
    int16_t _rate[3];
    int16_t _rateTemp[3];
    int16_t _boardTemp;
    uint16_t _counter;
    uint16_t _bitStatus;
    
    
    XbowSensorScaled1Pkt(sensor_scaled_data_t *data, system_data_t *sysData) : XbowPacket(XBOW_PROTOCOL, S1, 24) {
        
        // S1 packet data
        for (int i=0; i<3; i++) {
            _accel[i] = (int16_t) (data->accel[i] / MATH_CONSTANTS_G_MPS2 * ACCEL_SCALING);                        
            _rate[i] = (int16_t) (data->rate[i] * GYRO_SCALING);
            _rateTemp[i] = (int16_t) (data->rateTemp[i] * TEMP_SCALING);
        }
        
        _boardTemp = (int16_t) (data->accelTemp[0] * TEMP_SCALING);
        
        _counter = sysData->dataCounter;
        _bitStatus = 0;
    }
    
    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        for (int i=0; i<3; i++) {
            htonb_16(_accel[i]);
        }
        for (int i=0; i<3; i++) {
            htonb_16(_rate[i]);
        }
        
        for (int i=0; i<3; i++) {
            htonb_16(_rateTemp[i]);
        }
        htonb_16(_boardTemp);
        htonb_16(_counter);
        htonb_16(_bitStatus);
        
        return ESUCCESS;
    }
    
};

/**
* Scaled sensor 4 message
*/
class XbowSensorScaled4Pkt : public XbowPacket
{
  public:
    uint32_t _counter;
    float _rates[3];
    float _accls[3];
    float _encPos;
    int16_t _aTemp;
    uint16_t _sensorStatus;
    uint16_t _bitStatus;
    
    
    XbowSensorScaled4Pkt(sensor_scaled_data_t *sensorData, system_data_t *sysData) : XbowPacket(XBOW_PROTOCOL, S4, 38) {
        
        _counter = sysData->dataCounter;
        for (int i=0; i<3; i++) {
            _rates[i] = (float)sensorData->rate[i]; 
            _accls[i] = (float)sensorData->accel[i];
        }
        
        _aTemp = (int16_t)(sensorData->accelTemp[0] * S4_TEMP_SCALING);
        
        _encPos = (float) sensorData->encoderPosDegs * D2R;
        _sensorStatus = sysData->sensorStatus;
        _bitStatus = sysData->bitWord.all;
    }

	XbowSensorScaled4Pkt(uint8_t *bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, S4, 38) {
		_dPtr = bytes;
		ntohb_32(&_counter);
		for (int i = 0; i<3; i++) {
			ntonh_float(&_rates[i]);
		}

		for (int i = 0; i<3; i++) {
			ntonh_float(&_accls[i]);
		}

		ntonh_float(&_encPos);
		ntohb_16((uint16_t*)&_aTemp);
		ntohb_16(&_sensorStatus);
		ntohb_16(&_bitStatus);
	}

    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_32(_counter);
        for (int i=0; i<3; i++) {
            htonb_float(_rates[i]);
        }
                     
        for (int i=0; i<3; i++) {
            htonb_float(_accls[i]);
        }
        
        htonb_float(_encPos);
        htonb_16(_aTemp);
        htonb_16(_sensorStatus);
        htonb_16(_bitStatus);
        
        return ESUCCESS;
    }
    
};

/**
* Scaled sensor 5 message
*
* S5 is very similar to S4 except on S5: encoder is removed, x/y/z magnetometers are added
*/
class XbowSensorScaled5Pkt : public XbowPacket
{
  public:
    uint32_t _counter;
    float _rates[3];
    float _accls[3];
    float _mag[3];
    int16_t _aTemp;
    uint16_t _sensorStatus;
    uint16_t _bitStatus;
    
    
    XbowSensorScaled5Pkt(sensor_scaled_data_t *sensorData, system_data_t *sysData) : XbowPacket(XBOW_PROTOCOL, S5, 46) {
        
        _counter = sysData->dataCounter;
        for (int i=0; i<3; i++) {
            _rates[i] = (float)sensorData->rate[i]; 
            _accls[i] = (float)sensorData->accel[i];
            _mag[i] = (float)sensorData->mag[i];
        }
        
        _aTemp = (int16_t)(sensorData->accelTemp[0] * S4_TEMP_SCALING);
        
        _sensorStatus = sysData->sensorStatus;
        _bitStatus = sysData->bitWord.all;
    }

	XbowSensorScaled5Pkt(uint8_t *bytes, uint8_t payloadLen) : XbowPacket(XBOW_PROTOCOL, S5, 46) {
		_dPtr = bytes;
		ntohb_32(&_counter);
		for (int i = 0; i<3; i++) {
			ntonh_float(&_rates[i]);
		}

		for (int i = 0; i<3; i++) {
			ntonh_float(&_accls[i]);
		}

		for (int i = 0; i<3; i++) {
			ntonh_float(&_mag[i]);
		}

		ntohb_16((uint16_t*)&_aTemp);
		ntohb_16(&_sensorStatus);
		ntohb_16(&_bitStatus);
	}

    virtual error_t serialize(IDataStream *stream, ISerialFormat *formatter=NULL) {
        htonb_32(_counter);
        for (int i=0; i<3; i++) {
            htonb_float(_rates[i]);
        }
                     
        for (int i=0; i<3; i++) {
            htonb_float(_accls[i]);
        }
        
        for (int i=0; i<3; i++) {
            htonb_float(_mag[i]);
        }
        
        htonb_16(_aTemp);
        htonb_16(_sensorStatus);
        htonb_16(_bitStatus);
        
        return ESUCCESS;
    }

};

#endif  // _XBOW_SENSOR_SCALED_PKT_HPP_

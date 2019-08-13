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
#ifndef _MFTEST_PROTO_HPP
#define _MFTEST_PROTO_HPP

#include "ICustomStream.h"
#include "ISerial.hpp"
#include <stdint.h>

#define MFP_INP_BUF_SIZE    256

enum{
    MFP_STATE_START,
    MFP_STATE_DATA,
    MFP_STATE_ESC,
    MFP_STATE_READY,
    MFP_STATE_ERROR,
};

typedef struct {
	char  model[12];
	int   what;
	int   type;
	float val;
}sensor_data_t;



#define MFP_FLAG    0x7e
#define MFP_ESC     0x7d

typedef void (*cfun)(void);

class TestProto {
    uint8_t _inpBuf[MFP_INP_BUF_SIZE];
    int     _inState;
    uint8_t *_inPtr;
    int     _inLen;
    cfun    _fun;
//	char    _fileName[300];	
    
	sensor_data_t _data;

    SerialStream*  _stream;       ///< Serial IO stream
    bool  checkCrc(uint8_t *buf, int len);

public:
    TestProto(): _stream(NULL), _inLen(0), _fun(NULL), _inState(MFP_STATE_START){
		_data.val = -1;
	}
    int  init(SerialStream *stream){ _stream = stream ; return 0;}
	uint8_t  *getMessage(SerialStream *stream, uint32_t sync, int *crc, int *lenPtr = NULL);
	uint8_t  *getXbowMessage(SerialStream *stream, uint32_t sync, int *crc, int *lenPtr = NULL, int timeout = 1);
	int  detectComPorts(int *portNum, int *size);
	int  connect(char *name);
	void disconnect();
	
};

#define EXT_SENS_SNUM_SIZE	12
#define EXT_SENS_NAME_SIZE	12
#define EXT_SENS_UID_SIZE	16

enum {
    TEST_CMD_NONE,
    TEST_CMD_UID,               // 1 Get UID
    TEST_CMD_OSC_FREQ,          // 2 Get Oscillator Frequency
    TEST_CMD_REF_VOLT,          // 3 Get Reference Voltage
    TEST_CMD_BATT_VOLT,         // 4 Get Battery Voltage
    TEST_CMD_BATT_ID,           // 5 Get Battery ID
    TEST_CMD_ACCEL_DATA,        // 6 Get Accelerometer Data
    TEST_CMD_MAG_DATA,          // 7 Get Magnetometer Data
    TEST_CMD_GYRO_DATA,         // 8 Get Gyro Data
    TEST_CMD_LIGHT_DATA,        // 9 Get Light Sensor Data
    TEST_CMD_TEMP_DATA,         // 0xa Get Temperature Sensor Data
    TEST_CMD_BARO_DATA,         // 0xb Get Barometer Data
    TEST_CMD_GSM_START,         // 0xc Wakeup GSM modem
    TEST_CMD_GSM_STOP,          // 0xd Shutdown GSM modem
    TEST_CMD_GSM_INTF,          // 0xe Get GSM SIM number
    TEST_CMD_GSM_SIM,           // 0xf Get GSM SIM number
    TEST_CMD_GSM_IMEI,          // 0x10 Get GSM IMEI number
    TEST_CMD_GSM_RING_START,    // 0x11 Prepare for GSM ring test
    TEST_CMD_GSM_RING_RES,      // 0x12 Get GSM ring test result
    TEST_CMD_GPS_START,         // 0x13 Wakeup GPS receiver
    TEST_CMD_GPS_STOP,          // 0x14 Shutdown GPS receiver
    TEST_CMD_GPS_INTF,          // 0x15 Check GPS interface
    TEST_CMD_GPS_DATA,          // 0x16 Get GPS data
    TEST_CMD_CSTAT,             // 0x17 Get Charge Status
    TEST_CMD_LCD_ON,            // 0x18 Turn ON LCD
    TEST_CMD_LCD_OFF,           // 0x19 Turn OFF LCD
    TEST_CMD_PB_START,          // 0x1a Prepare to test pushbutton
    TEST_CMD_PB_RESULT,         // 0x1b Get Pushbutton test result
    TEST_CMD_LIGHT_INT_START,   // 0x1c Prepare to test Light Sensor interrupt
    TEST_CMD_LIGHT_INT_RES,     // 0x1d Get Light sensor interrupt test result
    TEST_CMD_ACCEL_INT_START,   // 0x1e Prepare to test Accelerometer interrupt
    TEST_CMD_ACCEL_INT_RES,     // 0x1f Get Accelerometer interrupt test result
    TEST_CMD_SDCARD,            // 0x20 Test SD card
    TEST_CMD_LED,               // 0x21 Test LED
    TEST_CMD_HUM,               // 0x22 Test HUMIDITY Sensor
    TEST_CMD_EXT,               // 0x23 Test Extension connector
    TEST_CMD_BOARD_REV,         // 0x24 Get board revision
    TEST_CMD_GSM_DEFAULTS,      // 0x25 GSM default values
    TEST_CMD_APP_REV,           // 0x26 Application software revision
    TEST_CMD_GSM_CELL,          // 0x27 Get GSM cell info result
    TEST_CMD_SET_SN,            // 0x28 Set Device SN
    TEST_CMD_GET_SN,            // 0x29 Get Device SN
    TEST_CMD_SHUTDOWN,          // 0x2a Power down mode
    TEST_CMD_PROBE_PROG,        // 0x2b Programming the probe
};

// External sensor errors
typedef enum {
    ESENS_FAIL          = 800,  ///< Generic external sensor error
    ESENS_NOT_CONNECTED = 801,  ///< Ext sensor not connected/disconnected
    ESENS_DISCONNECTED  = 802,  ///< Ext sensor connection failure
    ESENS_INVALID       = 803,  ///< Invalid sensor connected
    ESENS_UNSUP         = 804,  ///< Not supported sensor connected
    ESENS_READ          = 805,  ///< Sensor read error
    ESENS_ALLOC         = 806,  ///< Sensor allocation error
    ESENS_INFO          = 807,  ///< Sensor info error
    ESENS_BLOCKED       = 808,  ///< Sensor readinds blocked
    ESENS_UID           = 809,  ///< Sensor UID recovery failed
    ESENS_DISABLED      = 810,  ///< Ext sensor disabled
    ESENS_PROG          = 811,  ///< Ext sensor programming error
} err_xsensor_t;


#define  XSENS_TYPE_THERM 1
#define  XSENS_TYPE_RTD   2


#pragma pack(1)

// Dev Probe Prog response
typedef struct {
	uint8_t   cmd;
	uint8_t   sensWhat;		// what sensing - T - temperature; H - humidity, ...
	uint8_t   sensType;		// type of actual sensor - T - thermistor, R - RTD, ..
    float     val;			// reference value
    uint8_t   sn[EXT_SENS_SNUM_SIZE+1];
    uint8_t   name[EXT_SENS_NAME_SIZE+1];
}probe_prog_req_t;

// Dev Probe Prog response
typedef struct {
	uint8_t   resp;
    uint8_t   result;
	int32_t   error;
	uint8_t   sn[EXT_SENS_SNUM_SIZE+1];
    uint8_t   uid[EXT_SENS_UID_SIZE*2+1];
}probe_prog_resp_t;

#pragma pack()

#endif

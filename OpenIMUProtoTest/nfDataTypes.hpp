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
* Defines basic data types for the NF1000
*
* @file    nfDataTypes.hpp
*/

#ifndef _NF_DATA_TYPES_HPP_
#define _NF_DATA_TYPES_HPP_

#include <stdint.h>


//------------------------------------------------------------------------------
typedef enum { 
    INIT_STATE,
    INIT_ALIGN,
    PRE_ALIGN = 2,
    COARSE_ALIGN = 3,
    INIT_FINE_ALIGN = 5,  // Keep these numbers as is, so processing tools don't break
    FINE_ALIGN = 4,
    NAV = 6,
    INIT_NORTH_TRACKING = 7,
    ZUPT
} Nav_state_enum;

//------------------------------------------------------------------------------
enum {
    GYRO_X = 0,
    GYRO_Y1 = 1,  //YP Y+
    GYRO_Y2 = 2,  //YP2
    GYRO_Y3 = 3,  //YM Y-
    GYRO_Y4 = 4,  //YM2
    GYRO_Z = 5,
    NUM_GYROS = 6
};

typedef struct {
    int32_t counts[NUM_GYROS];        // counts, x, y (1,2,3,4), z
    int32_t temp[NUM_GYROS];          // counts
    int32_t filteredTemp[NUM_GYROS];  // counts, low-pass filtered
    uint32_t timeTag;                 // ms
} gyro_raw_t;


//------------------------------------------------------------------------------
enum {
    ACCEL_X = 0,
    ACCEL_Y = 1,
    ACCEL_Z = 2,
    NUM_ACCELS = 3
};

#define NUM_ACCEL_TEMP  2

typedef struct{
    int32_t counts[NUM_ACCELS];
    int32_t temp[NUM_ACCELS];
    int32_t filteredTemp[NUM_ACCELS];
    uint32_t timeTag;                // ms
} accel_raw_t;


//------------------------------------------------------------------------------
typedef struct{
    int32_t gyroCounts[3];
    int32_t acclCounts[3];
    int32_t magCounts[3];
    int32_t temp;
    uint32_t timeTag;                // ms
} mpu_raw_t;


//------------------------------------------------------------------------------
enum {
    MAG_X = 0,
    MAG_Y = 1,
    MAG_Z = 2,
    NUM_MAGS = 3
};

typedef struct{
    int32_t  counts[3];
    uint32_t temp;                   // temperature
    uint32_t timeTag;                // ms
} mag_raw_t;

//--------- NK (ANAV300) has 3 gyros -------------------------------------------
enum {
    GYRO_X_NK = 0,
    GYRO_Y_NK = 1,
    GYRO_Z_NK = 2,
    NUM_GYROS_NK = 3
};

typedef struct {
    int32_t  rateCounts[NUM_GYROS_NK];
    int32_t  acclCounts[NUM_ACCELS];
    int32_t  tempCount;
    int32_t  filteredTemp;
    uint32_t boardTemp;
    uint32_t timeTag;                 // ms
} nk_raw_t;

//------------------------------------------------------------------------------
typedef enum {
    ROTARY_INIT = 0,
    ROTARY_FAIL = 3,
    ROTARY_READY = 1,
    ROTARY_ENCODER_REPORT = 2,
    ROTARY_NOT_READY = 4
} rotary_status_t;

typedef struct {
    double encoderDegs;            ///< the rotary stage position, in degrees (from faulhaber)
    bool motorIsStopped;           ///< tracks stationary state of the motor
    uint32_t timeStampMs;          ///< encoder data time tag in ms
    bool posValid;                 ///< validity flag for position data
    rotary_status_t rotaryStatus;  ///< status of motor task /  rotary stage
} rotary_stage_data_t;


//------------------------------------------------------------------------------
// regardless of the number of gyros and accelrometers, they are combined to three axes measurements
typedef struct {
    double rate[3];                // radian/s, x, y, z
    double rateTemp[3];            // deg C, x, y, z
    double accel[3];               // m/s^2, x, y and z
    double accelTemp[3];           // 3 accel sensors, deg C
    uint32_t timeTag;              // ms
    double encoderPosDegs;
    int rotaryStageIsStationary;
    double mag[3];                 // Gauss
} sensor_scaled_data_t;


//------------------------------------------------------------------------------
// Sensor measurement data as used by the alignment filter
typedef struct {
    double rate[3];                // radian/s
    double accel[3];               // m/s^2, x, y and z
    double encoderPosRad;          // radians
} measurement_data_t;


//----------------------------------------------------------------------------- 
// position data expressed on WGS84
typedef struct {
    double latitude;  // in degree
    double longitude; // in degree
    double height;    // in meters
} llh_t;

//------------------------------------------------------------------------------
typedef struct {
    llh_t llh;        ///< degrees, degrees, meters WGS84 HAE
    double azimuth;   ///< rad, -pi to pi
    double pitch;     ///< rad, -pi/2 to pi/2
    double roll;      ///< rad, -pi to pi
    double eulerFom[3];  ///< Figure of Merit, in order azimuth, pitch, roll, units are rad
	Nav_state_enum navState;  // nav states
} nav_data_t;

//------------------------------------------------------------------------------
typedef struct {
    double attitude[3];       // rpy, rad
    double fom[3];            // rpy, rad
    double velNed[3];         // velociites, m/s
    llh_t llh;                // deg, deg, m 
    uint16_t navBit;
} nk_data_t;

//------------------------------------------------------------------------------
#ifndef __PAVAM

///< Example BIT Word per AVAM ICD v4.0 (p.14)
typedef struct {
    uint8_t CORE_TEMP_KO:1;
    uint8_t CORE_TEMP_GRAD_KO:1;
    uint8_t BOARD_TEMP_KO:1;
    uint8_t GYRO_KO:1;
    uint8_t ACCELERO_KO:1;
    uint8_t POWER_SUPPLY_S_KO:1;
    uint8_t POWER_SUPPLY_P_KO:1;
    uint8_t DSP_CORE_KO:1;
    uint8_t INIT_ERROR:1;
    uint8_t CMD_OK:1;
    uint8_t MSG_KO:1;
    uint8_t RESERVED:5;
} bits_t;

typedef union {
    uint16_t all;
    bits_t bits;
} bit_word_t;

///< Example Status Word per AVAM ICD v4.0 (p.14)
typedef struct {
    uint8_t MODE:2;
    uint8_t SUBMODE:2;
    uint8_t ALN_TYPE:2;
    uint8_t NK_KO:1;
    uint8_t NF_KO:1;
    uint8_t HH_PRESENT:1;
    uint8_t BORP_MODE:1;
    uint8_t NF_INITIALIZING:1;
    uint8_t RESERVED:5;
} status_t;
typedef union {
    uint16_t all;
    status_t status;
} status_word_t;

#else

#include "drsDataTypes.hpp"

#endif


typedef struct {
    double          modeTime;           // in seconds, t=0 means North-finding completed, t<0 means alignment in process
    double          systemTime;         // in seconds per AVAM ICD
    double          runTime;            // in seconds per PAVAM ICD
    status_word_t   statusWord;         // see AVAM ICD
    bit_word_t      bitWord;            // see AVAM ICD
    uint32_t        dataCounter;        // counter for data messages, also used in S1 output
    uint16_t        pioMode;            // mode of PIO board
    uint16_t        sensorStatus;       // sensor status per DMU Spec Amendments
    uint16_t        hwBitWord;
    uint16_t        swBitWord;
    uint16_t        sensorsBitWord;
    uint16_t        hwStatusWord;
    uint16_t        swStatusWord;
    uint16_t        sensorsStatusWord;
} system_data_t;


#define BIT_WORDS_MAX   40
typedef struct{
    uint32_t    runTime;
    uint16_t    tempKoCnt;
    uint16_t    numBitWords;
    uint16_t    bitWords[BIT_WORDS_MAX];
}bit_hist_t;


#endif  // _NF_DATA_TYPES_HPP_

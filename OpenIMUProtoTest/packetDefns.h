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
 * Definitions of packet types for the Messaging module
 *
 * @file    packetDefns.h
 */

#ifndef _PACKET_DEFNS_H_
#define _PACKET_DEFNS_H_


#define XBOW_PROTOCOL               0x5555
#define AVAM_CMD_PROTOCOL           0xCAFA
#define AVAM_RESPONSE_PROTOCOL      0xCAFB
#define DUAL_GYRO_PROTOCOL          0x0001
#define DRS_CMD_PROTOCOL            0x261A
#define DRS_RESPONSE_PROTOCOL       0x2619
#define MAX_VERSIONS                5
#define XBOW_VG700_CMD_PROTOCOL     0x6666
#define XBOW_VG700_DATA_PROTOCOL    0x6667

// @todo - tm20111025 - we may be able to delete some of these - at the very least, sort them numerically so you don't need ctrl-F to find a message

// DMU440/540 series packet types:
#define A0 0x4130  //Angle 0
#define A1 0x4131  //Angle 1
#define A2 0x4132  //Angle 2
#define A6 0x4136  //Angle 6
#define AR 0x4152  //Algorithm Reset
#define AT 0x4154  //Attitude transfer
#define B1 0x4231  //Beeline B1
#define B2 0x4232  //Beeline B2
#define CC 0x4343  //calibration complete
#define CH 0x4348  //echo command
#define D1 0x4431  // debug packet 1
#define D2 0x4432  // debug packet 2
#define D3 0x4433  // debug packet 3
#define D4 0x4434  // debug packet 4
#define D5 0x4435  // debug packet 5
#define D6 0x4436  // debug packet 6
#define D7 0x4437  // debug packet 7
#define D8 0x4438  // debug packet 8
#define D9 0x4439  // debug packet 9
#define F1 0x4631  //Factory 1
#define F2 0x4632  //Factory 2
#define F8 0x4638  //Factory 8
#define F9 0x4639  //Factory 9
#define FA 0x4641  //Factory 10
#define FB 0x4642  //Factory 11
#define FC 0x4643  //Factory 12
#define FD 0x4644  //Factory 13
#define FE 0x4645  //Factory 14 - raw sensors data from structure
#define FK 0x464b  //Factory raw NK
#define GF 0x4746  //Get fields
#define GP 0x4750  //Get packet
#define ID 0x4944  //Identification
#define KC 0x4b43  //Kernel CPU data	
#define KS 0x4b53  //Kernel Semaphore data
#define KT 0x4b54  //Kernel Tasks data
#define LEGACY500 0x0500 //legacy 500 packet
#define MC 0x4D43  //DMC
#define N0 0x4E30  //Nav 0
#define N1 0x4E31  //Nav 1
#define N2 0x4E32  //Nav 2
#define N3 0x4E33  //Nav 3
#define N4 0x4E34  //Nav 4
#define P1 0x5031  //PAVAM 1
#define PK 0x504B  //ping
#define PR 0x5052  //Programming Reset
#define RE 0x5245  //read EEPROM
#define RF 0x5246  //Read fields
#define S0 0x5330  //Scaled 0
#define S1 0x5331  //Scaled 1
#define S2 0x5332  //Scaled 2
#define S4 0x5334  //Scaled 2
#define SM 0x534D  //Scaled M
#define S5 0x5335  //Scaled 5
#define SA 0x5341  //Start attitude transfer
#define SE 0x5345  //Set EEPROM
#define SF 0x5346  //Set fields
#define SR 0x5352  //Software Reset
#define ST 0x5354  // start or stop TM packet
#define T0 0x5430  //Test 0 (Detailed BIT and Status)
#define T3 0x5433  //Test 3 (Detailed BIT and Status)
#define T1 0x5431  //Test 1 (Detailed BIT and Status)
#define TM 0x544D  // 1 PPS Time Mark Data Packet
#define TT 0x5454  // Time Transfer
#define U1 0x5531  //User 1
#define U2 0x5532  //User 2
#define UE 0x5545  //Unlock EEPROM
#define VA 0x5641  //Version All Data Packet
#define VR 0x5652  //Version Data Packet
#define WC 0x5743  //write calibration
#define WE 0x5745  //Write EEPROM
#define WF 0x5746  //Write fields
#define UE 0x5545  // unlock EEPROM 
#define LE 0x4C45  // lock EEPROM
#define JI 0x4A49  // JI
#define JA 0x4A41  // JA
#define WA 0x5741 

// Commands for OpenIMU
#define USR_IN_PING               0x7047   //  "pG"
#define USR_IN_UPDATE_CONFIG      0x7543   //  "uC" 
#define USR_IN_UPDATE_PARAM       0x7550   //  "uP" 
#define USR_IN_UPDATE_ALL         0x7541   //  "uA" 
#define USR_IN_SAVE_CONFIG	      0x7343   //  "sC" 
#define USR_IN_RESTORE_DEFAULTS   0x7244   //  "rD" 
#define USR_IN_GET_CONFIG		  0x6743   //  "gC" 
#define USR_IN_GET_PARAM		  0x6750   //  "gP" 
#define USR_IN_GET_ALL		      0x6741   //  "gA" 
#define USR_IN_RESET		      0x7253   //  "rS" 
#define USR_IN_GET_VERSION	      0x6756   //  "gV" 


#define USR_OUT_TEST             0x7A54    //  "zT" 
#define USR_OUT_DATA1            0x7A31    //  "z1" 
#define USR_OUT_DATA2            0x7A32    //  "z2" 


#define DRS_GET_RUNTIME     0x0001

// ---------------------------------------------------

#define NAK        0x1515  // NAK error
#define CE         0x4345  // DMC clibration complete

#define CMD_KO     0x4B4f  // User command failure


#endif  // _PACKET_DEFNS_H_

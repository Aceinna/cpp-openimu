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
 * Utility module providing checksum and crc functions
 *
 * @file    cksum.h
 */

#ifndef _CKSUM_H_
#define _CKSUM_H_

#include <stdint.h>

//------------------------------------------------------------------------------
/**
    * 16-bit CRC, see DMU Serial Interface Spec for more details
    *
    * @param buf  - the buffer of bytes for the crc
    * @param numBytes  - the number of bytes over which to take the crc
    *
    * @returns crc16
    */
//------------------------------------------------------------------------------
uint16_t crc16(unsigned char *buf, uint32_t numBytes);


//------------------------------------------------------------------------------
/**
    * Simple checksum function, per AVAM ICD
    *
    * @param buf  - the buffer of bytes for the checksum
    * @param numWords  - the number of 16-bit words in the message
    *
    * @returns crc16
    */
//------------------------------------------------------------------------------
uint16_t checksum16(unsigned char *buf,  uint16_t numWords);

//------------------------------------------------------------------------------
/**
    * Simple checksum function, per ...
    *
    * @param buf  - the buffer of bytes for the checksum
    * @param numBytes  - the number of bytes in the message
    *
    * @returns sum of bytes
    */
//------------------------------------------------------------------------------
uint8_t checksum8(unsigned char *buf,  int numBytes);

//###########################################################################
// FUNCTION:  calcCRC calculates a 2-byte CRC on serial data using the 
//            CRC-CCITT 16-bit standard maintained by the ITU 
//			 (International Telecommunications Union).  
//
// ARGUMENTS: buf points to the beginning of the CRC'ed data.
//            num is the number of words of data.
//
// RETURN:    2-byte CRC on buf.
//
// AUTHOR: Darren Liccardo, Dec. 2003
// Note: used for generate CRC for unlock the EEPROM for backward cmpatability of 
// test SW (DMUTest and NAVVIEW
//###########################################################################
uint16_t calcCRC(uint16_t *buf, uint16_t num);
#endif  // _CKSUM_H_

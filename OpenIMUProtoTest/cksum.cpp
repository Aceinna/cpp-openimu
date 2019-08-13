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


#include "cksum.h"

//------------------------------------------------------------------------------
uint16_t crc16(unsigned char *buf, uint32_t numBytes)
{
	uint16_t crc = 0x1D0F;  //non-augmented inital value equivalent to the augmented initial value 0xFFFF
	
	for (int i=0; i<numBytes; i++) {
		crc ^= buf[i] << 8;
		
		for (int j=0; j<8; j++) {
			if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            }
			else {
                crc = crc << 1;
            }
		}
	}
	
	return crc;
}


//------------------------------------------------------------------------------
uint16_t checksum16(unsigned char *buf,  uint16_t numWords)
{
    uint16_t cksum = 0;

    // Sum the words
    for (int i=0; i<numWords; i++) {
        uint16_t nextWord = (buf[2*i] << 8 & 0xff00) | (buf[2*i+1] & 0x00ff);
        cksum += nextWord;
    }

    // Take one's complement
    cksum = ~cksum;

    // Add one to get two's complement
    cksum += 1;

    return cksum;
}

//------------------------------------------------------------------------------
uint8_t checksum8(unsigned char *buf,  int numBytes)
{
    uint8_t cksum = 0;

    // Sum the words
    for (int i=0; i<numBytes; i++) {
        cksum += *buf++;
    }

    // Take one's complement
    cksum = ~cksum;

    // Add one to get two's complement
    cksum += 1;

    return cksum;
}

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
//###########################################################################
uint16_t calcCRC(uint16_t *buf, uint16_t num) {
	int i,j;
	uint16_t crc=0x1D0F;  //non-augmented inital value equivalent to the augmented initial value 0xFFFF
	
	for (i=0; i<num; i+=1) {
		crc ^= buf[i];
		
		for(j=0;j<16;j+=1) {
			if(crc & 0x8000) crc = (crc << 1) ^ 0x1021;
			else crc = crc << 1;
		}
	}
	
	return crc;
}
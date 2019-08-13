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
#include "stdafx.h"
#include "protocol.hpp"
#include "XbowPacket.hpp"
#include "cksum.h"
#include <time.h>

#define MSG_BUF_SIZE 1024

uint8_t msgBuf[MSG_BUF_SIZE];
uint8_t tempBuf[MSG_BUF_SIZE];
int	    bytesLeft = 0;
uint8_t *ptr;

uint16_t crc_byte(uint16_t crc, uint8_t b)
{
	uint8_t i = 8;

	crc = crc ^ b << 8;

	do{
		if (crc & 0x8000){
			crc = crc << 1 ^ 0x1021;
		}
		else{
			crc = crc << 1;
		}
	} while (--i);

	return crc;

}



uint16_t calcCrc(uint8_t *buf, int len)
{
	uint16_t crc = 0x1D0F;

	for (int i = 0; i < len; i++){
		crc = crc_byte(crc, *buf);
		buf++;
	}

	return crc;
}



bool  TestProto::checkCrc(uint8_t *buf, int len)
{
    uint16_t crc = 0, msgCrc;

    if(len < 3){
        return false;
    }

    len -=2;

    for(int i=0; i < len; i++, buf++){
        crc += *buf;
    }

    msgCrc = ((*(buf+1)) << 8) | (*(buf));

    return msgCrc == crc;

}


uint8_t	*TestProto::getMessage(SerialStream *stream, uint32_t sync, int *crc, int *lenPtr)
{
	uint8_t  *rptr;
	uint8_t  tmp, synced;
	int      msgLen, found, len, state, res;
	uint32_t test_sync;
	uint16_t crcMsg, crcCalc, numWords;
	time_t	 currentTime, startTime = time(NULL), diff;
	state = 0;
	synced = 0;
	test_sync = 0;
	len = 0;
	found = 0;
	*crc = 0;

	while (len < MSG_BUF_SIZE){
		if (bytesLeft == 0){
			currentTime = time(NULL);
			diff = currentTime - startTime;
			if (diff > 10){
					return NULL;
			}
			res = stream->read(tempBuf, 10);
			if (res < 0){
				return NULL;
			}
			if (res == 0){
				if (currentTime - startTime > 10){
					return NULL;
				}
				continue;
			}
			bytesLeft = res;
			ptr       = tempBuf;
		}
		tmp = *ptr++;
		bytesLeft--;
		test_sync = (test_sync << 8) | tmp;
		if (test_sync == sync){
			state = 0;
			len = 4;
			msgBuf[0] = (sync >> 24) & 0xff;
			msgBuf[1] = (sync >> 16) & 0xff;
			msgBuf[2] = (sync >> 8) & 0xff;
			msgBuf[3] = (sync)& 0xff;
			rptr = msgBuf + 4;
			crcMsg = 0;
			msgLen = 0;
			numWords = 0;
			synced = 1;
			continue;
		}
		if (!synced){
			continue;
		}
		*rptr++ = tmp;
		len++;
		switch (state){
		case 0:
			numWords = (numWords << 8) | tmp;
			if (len == 6){
				msgLen = numWords * 2;
				if (msgLen > MSG_BUF_SIZE - 8){
					synced = 0;
					break;
				}
				state = 1;
			}
			break;
		case 1:
			if (len == msgLen + 6){
				state = 2;
				crcMsg = 0;
			}
			break;
		case 2:
			crcMsg = tmp;
			state = 3;
			break;
		case 3:
			state = 0;
			synced = 0;
			crcMsg = crcMsg << 8 | tmp;
			crcCalc = checksum16(msgBuf, numWords + 3);
			if (crcMsg == crcCalc){
				found = 1;
			}
			else{
				*crc = 1;
			}
			break;
		default:
			break;
		}
		if (found){
			break;
		}
	}
	return msgBuf + 6;
}



uint8_t	*TestProto::getXbowMessage(SerialStream *stream, uint32_t sync, int *crc, int *lenPtr, int timeout)
{
	uint8_t  *rptr;
	uint8_t  tmp, synced;
	int      msgLen, found, len, state, res;
	uint32_t test_sync;
	uint16_t crcMsg, crcCalc, numBytes;
	time_t	 currentTime, startTime = time(NULL), diff;
	state = 0;
	synced = 0;
	test_sync = 0;
	len = 0;
	found = 0;
	*crc = 0;
	if (lenPtr) {
		*lenPtr = 0;
	}

	while (len < MSG_BUF_SIZE){
		if (bytesLeft == 0){
			currentTime = time(NULL);
			diff = currentTime - startTime;
			if (diff > timeout){
				return NULL;
			}
			res = stream->read(tempBuf, 10);
			if (res < 0){
				return NULL;
			}
			if (res == 0){
				if (currentTime - startTime > timeout){
					return NULL;
				}
				continue;
			}
			bytesLeft = res;
			ptr = tempBuf;
		}
		tmp = *ptr++;
		bytesLeft--;
		test_sync = (test_sync << 8) | tmp;
		if (test_sync == sync){
			state = 0;
			len = 4;
			msgBuf[0] = (sync >> 24) & 0xff;
			msgBuf[1] = (sync >> 16) & 0xff;
			msgBuf[2] = (sync >> 8) & 0xff;
			msgBuf[3] = (sync)& 0xff;
			rptr = msgBuf + 4;
			crcMsg = 0;
			msgLen = 0;
			numBytes = 0;
			synced = 1;
			continue;
		}
		if (!synced){
			continue;
		}
		*rptr++ = tmp;
		len++;
		switch (state){
		case 0:
			msgLen = tmp;
			if (msgLen > MSG_BUF_SIZE - 8){
				synced = 0;
				break;
			}
			if (msgLen == 0){
				state = 2;
				break;
			}
			state = 1;
			break;
		case 1:
			if (len == msgLen + 5){
				state = 2;
				crcMsg = 0;
			}
			break;
		case 2:
			crcMsg = tmp;
			state = 3;
			break;
		case 3:
			state = 0;
			synced = 0;
			crcMsg = crcMsg << 8 | tmp;
			crcCalc = calcCrc(msgBuf+2, msgLen + 3);
			if (crcMsg == crcCalc){
				found = 1;
				if (lenPtr) {
					*lenPtr = msgLen;
				}
			}
			else{
				*crc = 1;
			}
			break;
		default:
			break;
		}
		if (found){
			break;
		}
	}
	
	return msgBuf + 5;
}




int TestProto::detectComPorts(int *portNum, int *size)
{
	char name[20];
	SerialStream *stream;
	int num = *size;
	*size = 0;

	for(int i = 1; i < num; i++){
		sprintf(name, "COM%d", i);
		stream = new SerialStream(name, 115200);
		if(stream->isValid()){
			stream->close();
			*portNum++ = i;
			(*size)++;
		}
		delete(stream);
	}
	
	return 0;

}

int TestProto::connect(char *name)
{
	SerialStream *stream;

	stream = new SerialStream(name, 115200);
	if(!stream->isValid()){
		delete(stream);
		return -1;
	}
	_stream = stream;

	return 0;
}

void TestProto::disconnect()
{
	ICustomStream *stream;

	if(_stream){
		_stream->close();
		delete(stream);
		_stream = NULL;
	}
}


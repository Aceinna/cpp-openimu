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

#ifndef _ICUSTOM_STREAM_H
#define _ICUSTOM_STREAM_H
#include "stdint.h"

#define INFINITE_TIMEOUT 60000		// one minute

class ICustomStream{

public:
	virtual int read(unsigned char* buf, int len, uint32_t timeout = INFINITE_TIMEOUT)  = 0;
	virtual int write(unsigned char* buf, int len, uint32_t timeout = INFINITE_TIMEOUT) = 0;
	virtual int readByte(unsigned char* buf, int timeout = INFINITE_TIMEOUT){
		return read(buf, 1, timeout);
	};
	virtual int writeByte(unsigned char byte, int timeout = INFINITE_TIMEOUT){
		return write(&byte, 1, timeout);
	};

	virtual int close() = 0;
	virtual bool isValid() = 0;
};


#endif

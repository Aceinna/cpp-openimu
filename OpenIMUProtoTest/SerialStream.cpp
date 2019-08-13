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

#include "SerialStream.hpp"

int SerialStream::read(unsigned char* buf, int len, uint32_t timeout)
{
	int count;
	bool res;

	res = ReadFile(m_handle, buf, len, (LPDWORD)&count, NULL); // write is updated with the number of bytes written
	if(!res){
		printf("com read error\r\n");
		return -11;		
	}
	
	return count;	
}

int SerialStream::write(unsigned char* buf, int len, uint32_t timeout)
{
	int count;
	bool res;

	res = WriteFile(m_handle, buf, len, (LPDWORD)&count, NULL); // write is updated with the number of bytes written
	
	if(!res || count == 0){
		printf("com write error\r\n");
		return -1;		
	}
	
	return count; 
}

SerialStream::SerialStream(char* name, int baudrate)
{
	char port[100]; 
	
	m_valid  = false;

	sprintf(port, "%s", SERIAL_PORT_PREFFIX);
	strcat(port, name);

	//	port = argv[3];

	m_handle = CreateFileA(port,  GENERIC_READ|GENERIC_WRITE,
                          0,NULL,
                          OPEN_EXISTING,0,NULL);

    if (m_handle == INVALID_HANDLE_VALUE) {
        printf("Failed to open port\r\n");
		return;
	} else {
// set timeouts
        COMMTIMEOUTS cto = { 1, 100, 100, 10, 10};
        DCB dcb;
        if (!SetCommTimeouts(m_handle, &cto)){
            printf("SetCommTimeouts failed\r\n");
			CloseHandle(m_handle);
			return;
		}

// set DCB
        memset(&dcb,0,sizeof(dcb));

        dcb.DCBlength   = sizeof(dcb);
        dcb.BaudRate    = baudrate;
        dcb.fBinary     = 1;
        dcb.fDtrControl = DTR_CONTROL_DISABLE;
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
        dcb.Parity      = NOPARITY;
        dcb.StopBits    = ONESTOPBIT;
        dcb.ByteSize    = 8;

        if (!SetCommState(m_handle, &dcb)){
            printf("SetCommState failed");
			CloseHandle(m_handle);
			return;
		}
    }

	m_valid  = true;
}

int SerialStream::close()
{
	CloseHandle(m_handle);
	return 0;
}

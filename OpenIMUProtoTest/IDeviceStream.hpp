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
 * C++ interface for device byte streams.
 *
 * @file    IDeviceStream.hpp
 */

#ifndef __IDEVICESTREAM_HPP__
#define __IDEVICESTREAM_HPP__

#include "IDataStream.hpp"
#include "IDevice.hpp"
#include <stdint.h>

#define IOCTL_CMD_SET_BADDR 1	// set device bus address
#define IOCTL_CMD_SET_IADDR 2	// set device internal address to access to	

typedef struct{
    uint8_t addr;

}stream_baddr_t;


typedef struct{
    uint32_t iAddr;	// internal address
    int      iAddrSize; // internal address size in bytes

}stream_iaddr_t;

/**
 * C++ interface stream devicess.
 */

class IDeviceStream : public IDataStream, public IDevice{

public:
    IDeviceStream(bool shareable = false): IDevice(shareable){};
    /**
     * Generic ioctl function which sets up device-specific parameters.
     * @param cmdId ioctl command.
     * @param paramStruct - command parameters structure.
     * @param paramLen    - command parameters structure len.
     * @returns ESUCCESS in case of success, EFAIL otherwise).
     */
    virtual int ioctl(int cmdId, void *paramStruct, int paramLen) {return -EFAIL;}

};

#endif

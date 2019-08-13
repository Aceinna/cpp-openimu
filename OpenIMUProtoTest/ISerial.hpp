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
 * Base Class template for serial channels.
 *
 * @file    ISerial.hpp
 */

#ifndef __ISERIAL_HPP__
#define __ISERIAL_HPP__
#include <stdint.h>

#include "IDeviceStream.hpp"

/**
 * C++ class for managing a Serial bus.
 * Base class for all serial ports
 */

#define SERIAL_MODE_NORMAL       0
#define SERIAL_MODE_HW_CTRL      1
#define SERIAL_MODE_RS485        2

class ISerial : public IDeviceStream {
  public:
    ~ISerial() {}

    // == Abstract API ==
    virtual int      init(int baudrate = 0, int mode = SERIAL_MODE_NORMAL) = 0;
    virtual void     reset()   = 0;
    virtual void     disable() = 0;
    virtual void     enable()  = 0;
    virtual void     procIsr(XStream *pStream) = 0;
    virtual void     setSource(int busAddr, int devAddr) {};
    virtual void     setDestination(int busAddr, int devAddr) {};
    virtual void     loopback(bool fOn) {};

    // == IStream API ==
    virtual error_t   open(char *resourceName = NULL, uint32_t mode = 0);
    virtual error_t   close();
    //virtual int      read(uint8_t *buf, int len, int timeout = 0) = 0;
    //virtual int      write(uint8_t *buf, int len) = 0;
};

// ===== INLINE IMPLEMENTATIONS =====

/**
 * Opens the stream; claims underlying resources for reading and writing.
 * @param resourceName The name of the stream to open.
 * @returns error as error_t
 */
inline error_t ISerial::open(char *resourceName, uint32_t mode) {
  enable();
  return ESUCCESS;
}

/**
 * Closes the stream; releases underlying resources.
 * @returns error as error_t
 */
inline error_t ISerial::close() {
  disable();
  return ESUCCESS;
}


#endif

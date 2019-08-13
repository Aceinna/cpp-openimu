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
 * C++ interface for byte streams.
 *
 * @file    IStream.hpp
 */

#ifndef __XSTREAM_HPP__

#include <stdio.h>
#include <stdint.h>
#include "os.hpp"
#include "error.h"


/**
 * C++ interface for byte streams.
 */
class XStream {
  public:
    ~XStream() {}

    // == Abstract API ==

    /**
     * Reads up to len bytes from a stream into buf.
     * @returns number of bytes read or error code (negative).
     */
    virtual int read(uint8_t  *buf, int len, uint32_t timeout = Task::DELAY_INFINITE) = 0;

    /**
     * Writes up to len bytes to the stream from buf.
     * @returns number of bytes written or error code (negative).
     */
    virtual int write(uint8_t *buf, int len, uint32_t timeout = Task::DELAY_INFINITE) = 0;


    /**
     * Opens the stream; claims underlying resources for reading and writing.
     * @param resourceName The name of the stream to open.
     * @returns error as error_t
     */
    virtual error_t open(char *resourceName = NULL, uint32_t mode = 0){return ESUCCESS;}

    /**
     * Closes the stream; releases underlying resources.
     * @returns error as error_t
     */
    virtual error_t close(){return ESUCCESS;}

};

#endif

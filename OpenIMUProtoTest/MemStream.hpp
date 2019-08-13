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
 * A general class for providing a stream interface to a fixed size 
 * memory buffer.
 *
 * @file    MemStream.hpp
 */
#ifndef _MEMSTREAM_HPP
#define _MEMSTREAM_HPP

#include "error.h"

class MemStream : public IDataStream
{
    uint8_t   *_buffer;               ///< # bytes in buffer; index to next
    uint32_t  _size;                  ///< # bytes in buffer; index to next
    uint8_t   _currentPosition;       ///< current index, for read

public:
    MemStream(uint8_t *addr, int len) : _buffer(addr), _size(len), _currentPosition(0) {};

    /// Returns the raw bytes of the buffer.
    uint8_t *getBuffer() { return _buffer; }
    uint32_t getSize()   { return _size; }

    // == IStream API ==
    int  read  (uint8_t *buf, int len=1, unsigned timeout=Task::DELAY_INFINITE);
    int  write (uint8_t *buf, int len=1, unsigned timeout=Task::DELAY_INFINITE);
    
    error_t open(char *resourceName = NULL, uint32_t mode = 0) { return - EFAIL; }
    error_t reopen()  { _currentPosition = 0; return ESUCCESS; }
    error_t close() { return - EFAIL; }
    
};

/**
 * Provides a stream interface to write bytes to a buffer.
 */
inline int MemStream::write(uint8_t *buf, int len, uint32_t timeout)
{
    int num = 0;
    
    for (int i =_currentPosition; i < _size && len > 0; i++, len--) {
        _buffer[i] = *buf++;
        num++;    
        _currentPosition++;
    }
    
    return num;
}

/**
 * Stream interface for reading bytes from the stream to a buffer
 */

inline int MemStream::read(uint8_t *buf, int len, uint32_t timeout)
{
    int num = 0;
    
    for (int i =_currentPosition; i < _size && len > 0; i++, len--) {
        *buf++ = _buffer[i];
        num++;    
        _currentPosition++;
    }
    
    return num;
}

#endif

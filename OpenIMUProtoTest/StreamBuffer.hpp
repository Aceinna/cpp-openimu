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
 * @file    StreamBuffer.hpp
 * $Id$
 */
#ifndef _STREAM_BUFFER_HPP
#define _STREAM_BUFFER_HPP

#include "error.h"

#define STREAM_BUFFER_SIZE   280

template <int TBufferSize = STREAM_BUFFER_SIZE>
class StreamBuffer : public IDataStream
{
    uint32_t  _size;                  ///< # bytes in buffer; index to next
    uint8_t   _buffer[TBufferSize];   ///< the buffer itself
    uint8_t   _currentPosition;       ///< current index, for read

public:
    StreamBuffer() { clear(); }

    /// Resets the internal buffer to the beginning.
    void clear() { _size = 0; _currentPosition = 0; }

    /// Returns the raw bytes of the buffer.
    uint8_t *getBytes() { return _buffer; }
    uint32_t getSize()  { return _size; }

    // == IStream API ==
    int  read  (uint8_t *buf, int len=1, unsigned timeout=Task::DELAY_INFINITE);
    int  write (uint8_t *buf, int len=1, unsigned timeout=Task::DELAY_INFINITE);
    
    error_t open(char *resourceName = NULL, uint32_t mode = 0) { return - EFAIL; }
    error_t close() { return - EFAIL; }
    
};

/**
 * Provides a stream interface to write bytes to a buffer.
 */
template <int TBufferSize>
inline int StreamBuffer<TBufferSize>::write(uint8_t *buf, int len, uint32_t timeout)
{
    int i;
    for (i=0; i<len; i++) {
        if (_size >= TBufferSize) return -EFAIL;
        _buffer[_size++] = *buf++;
    }
    return i;
}

/**
 * Stream interface for reading bytes from the stream to a buffer
 */
template <>
inline int StreamBuffer<>::read(uint8_t *buf, int len, uint32_t timeout)
{
    int i;
    for (i=0; i<len; i++) {
        if (_currentPosition >= _size) return -EFAIL;
        *buf++ = _buffer[_currentPosition++];
    }
    
    return i;
}

#endif

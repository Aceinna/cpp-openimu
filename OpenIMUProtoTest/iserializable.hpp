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
 * @file    ISerializable.hpp
 */

#ifndef __ISERIALIZABLE_HPP__
#define __ISERIALIZABLE_HPP__

#include "IDataStream.hpp"

/**
 * C++ interface for classes that specify how to write out and parse 
 * a serialization format.
 */
class ISerialFormat {
  public:
    ~ISerialFormat() {}

    // Sample API.  Exact details TBD.  Formatter would interface with actual
    // object implementing ISerializable and convert name,type,value parameters
    // into underlying format.  Possibly, could be driven by "readNext" method
    // that returns typed parameter object.

    virtual error_t readHeader(IDataStream *stream) = 0;
    virtual error_t readBody(IDataStream *stream) = 0;
    virtual error_t readTail(IDataStream *stream) = 0;

    virtual error_t writeHeader(IDataStream *stream) = 0;
    virtual error_t writeBody(IDataStream *stream) = 0;
    virtual error_t writeTail(IDataStream *stream) = 0;
};

/**
 * C++ interface for byte streams.
 */
class ISerializable {
  public:
    ~ISerializable() {}

    // == Abstract API ==

    /**
     * Save this object and state contained into the given stream.
     *
     * @param  stream 	  This is the stream to which to write object data
     * @param  formatter  Optional for objects that can write various formats.
     * @returns ESUCCESS or -error_t code
     */
    virtual error_t serialize(IDataStream *stream,
                              ISerialFormat *formatter=NULL) {return -EFAIL;}

    /**
     * Load this object with instance data and state contained in the 
     * given stream.
     *
     * @param  stream 	  This is the stream from which to read object data
     * @param  formatter  Optional for objects that can read various formats.
     * @returns ESUCCESS or -error_t code
     */
    virtual error_t deserialize(IDataStream *stream,
                                ISerialFormat *formatter=NULL) { return -EFAIL;}
    /**
     * Deserealizes data from buffer 
     *
     * @param  data 	  data buffer
     * @param  len        data length
     * @returns ESUCCESS or -error_t code
     */
    virtual error_t deserialize(uint8_t *data, int len){return -EFAIL;}

};

#endif

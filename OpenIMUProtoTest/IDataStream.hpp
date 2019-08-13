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
 * @file    IDataStream.hpp
 */

#ifndef __IDATASTREAM_HPP__
#define __IDATASTREAM_HPP__

#include <stdint.h>
#include <string.h>
#include "error.h"
#include "byte_util.h"
#include "XStream.hpp"

/**
 * C++ interface for byte streams.
 */
class IDataStream : public XStream {

protected:
  
    uint32_t    _rTimeout;  // read timeout
    uint32_t    _wTimeout;  // write timeout

public:
    IDataStream():_rTimeout(Task::DELAY_INFINITE), _wTimeout(Task::DELAY_INFINITE) {}

    // == Derived API ==

    /**
     * Set read timeout.
     * @return ESUCCESS
     */
    int     setReadTimeout(uint32_t timeout)    { _rTimeout = timeout; return ESUCCESS;}

    /**
     * Set read/write timeout.
     * @return ESUCCESS
     */
    int     setWriteTimeout(uint32_t timeout)    { _wTimeout = timeout; return ESUCCESS;}


    int     setTimeout(uint32_t timeout) { 
        setReadTimeout(timeout); 
        setWriteTimeout(timeout); 
        return ESUCCESS;
    }

    /**
     * writes data to the end of the stream
     * @param buf  - pointer to data to be written
     * @param buf  - number of bytes to be written
     * @returns number of bytes transfered or -EFAIL
     * implementation is class-dependent, for serial stream - just write
     */
    virtual int   append(uint8_t *buf, int len, uint32_t timeout){
        return write(buf, len, timeout);
    }

    /**
     * formats data and writes data to the end of the stream
     * @param *format  - format string pointer
     * @returns number of bytes transfered or -EFAIL
     * implementation is class-dependent
     */
    virtual int writeF(const char *format, ...){ return -EFAIL;}

    /**
     * Synchronizes the buffer associated with the stream to its
     * controlled output sequence. This effectively means that all unwritten
     * characters in the buffer are written to its controlled output sequence
     * as soon as possible ("flushed").
     * @returns number of bytes flushed or -EFAIL
     */
    virtual int flush(){return -EFAIL;}
    /**
     * reads data into low level buffer, returns pointer to data start and number of bytes ready
     * @param buf - pointer to low-level buffer, which contains data
     * @param len - pointer to number of in level buffer
     * @returns ESUCECSS orr -EFAIL
     * implementation is class-dependent
     */
    virtual int readBuf(uint8_t **buf, int *len){return -EFAIL;}


    int readByte();
    int readShort();
    int readShortLE();
    long readInt();
    long readIntLE();
    float readFloat();
    float readFloatLE();

    error_t writeByte(uint8_t d);
    error_t writeInt16(int16_t d);
    error_t writeShort(uint16_t d);
    error_t writeShortLE(uint16_t d);
    error_t writeInt32(int32_t d);
    error_t writeUint(uint32_t d);
    error_t writeUintLE(uint32_t d);
    error_t writeFloat(float fl);
    error_t writeString(char* s, int n);

    /**
     * reads line from existing file
     * @param buf - buffer to put new line into
     * @param size - buffer size
     * @returns number of bytes transfered or -EFAIL
     */
    int     readLine(char *buf,  int size){
        int len = 0, res;
        while(len < size -1 && (res = read((uint8_t *)buf,1)) > 0){
            len++;
            if(*buf == '\n'){
                *(++buf) = 0;
                return len;
            }
            buf++;
            res = 0;
        }

        return res;
    };

    /**
     * writes string into the file. Adds \r\n after
     * @param buf  - pointer to string to be written
     * @returns number of bytes transfered or -EFAIL
     */
    int     writeLine(char *buf){
        char crlf[3] = "\r\n";
        int res, count = 0, len = strlen(buf);
        while(len){
            res = write((uint8_t *)buf, len);
            if(res <= 0){
                return res;
            }
            len   -= res;
            buf   += res;
            count += res;
        }
        len = 2;
        buf = crlf;
        while(len){
            res = write((uint8_t *)buf, len);
            if(res <= 0){
                return res;
            }
            len   -= res;
            buf   += res;
            count += res;
        }
        return count;
    };

    /**
     * controls data pointer
     * @param offset - number of bytes to shift from "from" origin
     * @param from   - origin from which offset applied
     * @returns ESUCCESS or  -EFAIL
     */
    virtual int     seek(int offset, int from) {return -EFAIL;}

};

// ===== READ UTILITIES =====

/**
 * Read next byte from the stream.
 * @return next byte (positive) or error_t code (negative).
 */
inline int IDataStream::readByte() {
  uint8_t b;
  int error = read(&b, 1, _rTimeout);
  return (error == 1) ? b : (error <= 0) ? error : -EFAIL;
}

/**
 * Read next two bytes from the stream in network format (big endian).
 * @return next short (positive) or error_t code (negative).
 */
inline int IDataStream::readShort() {
  uint8_t d[2];
  int error = read(d, 2, _rTimeout);
  return (error == 2) ? bpack2(d[0],d[1]) : (error <= 0) ? error : -EFAIL;
}

/**
 * Read next two bytes from the stream in native format (little endian).
 * @return next short (positive) or error_t code (negative).
 */
inline int IDataStream::readShortLE() {
  uint8_t d[2];
  int error = read(d, 2, _rTimeout);
  return (error == 2) ? bpack2(d[1],d[0]) : (error <= 0) ? error : -EFAIL;
}

/**
 * Read next four bytes from the stream in network format (big endian).
 * @return next int (positive) or error_t code (negative).
 */
inline long IDataStream::readInt() {
  uint8_t d[4];
  int error = read(d, 4, _rTimeout);
  return (error == 4) ? bpack4(d[0],d[1],d[2],d[3]) : (error <= 0) ? error : -EFAIL;
}

/**
 * Read next four bytes from the stream in native format (little endian).
 * @return next int (positive) or error_t code (negative).
 */
inline long IDataStream::readIntLE() {
  uint8_t d[4];
  int error = read(d, 4, _rTimeout);
  return (error == 4) ? bpack4(d[3],d[2],d[1],d[0]) : (error <= 0) ? error : -EFAIL;
}

/**
 * Read next four bytes from the stream in network format (big endian).
 * @return next float.
 */
inline float IDataStream::readFloat() {
    union FloatConverter_t {
        float fVal;
        int uVal;
    };

    FloatConverter_t bytes;
    
    uint8_t d[4];
    int error = read(d, 4, _rTimeout);
    bytes.uVal = bpack4(d[0],d[1],d[2],d[3]);
    return bytes.fVal;
}

/**
 * Read next four bytes from the stream in native format (little endian).
 * @return next float.
 */
inline float IDataStream::readFloatLE() {
    union FloatConverter_t {
        float fVal;
        int uVal;
    };

    FloatConverter_t bytes;
    
    uint8_t d[4];
    int error = read(d, 4, _rTimeout);
    bytes.uVal = bpack4(d[3],d[2],d[1],d[0]);
    return bytes.fVal;
}

// ===== WRITE UTILITIES =====

/**
 * Write byte to a stream.
 * @param b byte to send.
 * @return ESUCCESS or error (negative) or incomplete write (positive)
 */
inline error_t IDataStream::writeByte(uint8_t b) {
  int error = write(&b, 1, _wTimeout);
  return (error == 1) ? ESUCCESS : (error != 0) ? error : -EFAIL;
}

/**
 * Write signed 16-bit int to a stream in network order (big endian).
 * @param d int16_t to send.
 * @return ESUCCESS or error (negative) or incomplete write (positive)
 */
inline error_t IDataStream::writeInt16(int16_t d) {
    uint8_t buf[2] = { b2(d), b1(d) };
    int error = write(buf, 2, _wTimeout);
    return (error == 2) ? ESUCCESS : (error != 0) ? error : -EFAIL;
}

/**
 * Write short to a stream.
 * @param d uint16_t to send.
 * @return ESUCCESS or error (negative) or incomplete write (positive)
 */
inline error_t IDataStream::writeShort(uint16_t d) {
  uint8_t buf[2] = { bhi(d), blo(d) };
  int error = write(buf, 2, _wTimeout);
  return (error == 2) ? ESUCCESS : (error != 0) ? error : -EFAIL;
}

/**
 * Write short to a stream as little endian.
 * @param d uint16_t to send.
 * @return ESUCCESS or error (negative) or incomplete write (positive)
 */
inline error_t IDataStream::writeShortLE(uint16_t d) {
  uint8_t buf[2] = { blo(d), bhi(d) };
  int error = write(buf, 2, _wTimeout);
  return (error == 2) ? ESUCCESS : (error != 0) ? error : -EFAIL;
}

/**
 * Write signed 32-int to a stream in network order (big endian).
 * @param d int32_t to send.
 * @return ESUCCESS or error (negative) or incomplete write (positive)
 */
inline error_t IDataStream::writeInt32(int32_t d) {
    uint8_t buf[4] = { b4(d), b3(d), b2(d), b1(d) };
    int error = write(buf, 4, _wTimeout);
    return (error == 4) ? ESUCCESS : (error != 0) ? error : -EFAIL;
}

/**
 * Write unsigned int to a stream in network order (big endian).
 * @param d uint32_t to send.
 * @return ESUCCESS or error (negative) or incomplete write (positive)
 */
inline error_t IDataStream::writeUint(uint32_t d) {
  uint8_t buf[4] = { b4(d), b3(d), b2(d), b1(d) };
  int error = write(buf, 4, _wTimeout);
  return (error == 4) ? ESUCCESS : (error != 0) ? error : -EFAIL;
}

/**
 * Write unsigned int to a stream as little endian.
 * @param d uint32_t to send.
 * @return ESUCCESS or error (negative) or incomplete write (positive)
 */
inline error_t IDataStream::writeUintLE(uint32_t d) {
  uint8_t buf[4] = { b1(d), b2(d), b3(d), b4(d) };
  int error = write(buf, 4, _wTimeout);
  return (error == 4) ? ESUCCESS : (error != 0) ? error : -EFAIL;
}

/**
 * Write 4-byte float to a stream in network order (big endian).
 * @param f float to send.
 * @return ESUCCESS or error (negative) or incomplete write (positive)
 */
inline error_t IDataStream::writeFloat(float fl) {
    union FloatConverter_t {
        float fVal;
        uint32_t uVal;
    };

    FloatConverter_t bytes;
    bytes.fVal = fl;

    uint8_t buf[4] = { bytes.uVal >> 24, bytes.uVal >> 16, bytes.uVal >> 8, bytes.uVal };
    int error = write(buf, 4, _wTimeout);
    return (error == 4) ? ESUCCESS : (error != 0) ? error : -EFAIL;
}

/**
 * Write string to a stream in network order (big endian).
 * @param s string to send
 * @param n length of string
 * @return ESUCCESS or error (negative) or incomplete write (positive)
 */
inline error_t IDataStream::writeString(char* s, int n) {
    int error = 0;
    for (int i=0; i<n; i++) {
        error = writeByte(s[i]);
    }
    return (error == n) ? ESUCCESS : (error != 0) ? error : -EFAIL;
}

#endif

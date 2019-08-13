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
 * A collection of utility functions for byte manipulation of standard types.
 *
 * @file    byte_util.h
 *
 */

#ifndef _BYTE_UTIL_H_
#define _BYTE_UTIL_H_

#include <stdint.h>

/// Utility to extract high byte of uint16_t.
inline uint8_t bhi(uint16_t w) { return ((w >> 8) & 0xFF); }

/// Utility to extract low byte of uint16_t.
inline uint8_t blo(uint16_t w) { return (w & 0xFF); }

/// Utility to pack two bytes into high and low of short uint16_t.
inline uint16_t bpack2(uint8_t hi, uint8_t lo) {
    return ((hi << 8) | lo);
}

/// Utility to pack two bytes into high and low of short uint16_t.
inline uint16_t bflip2(uint16_t w) {
    return bpack2(blo(w),bhi(w));
}

/// Utility to extract first lowest byte of uint32_t.
inline uint8_t b1(uint32_t w) { return (w & 0xFF); }

/// Utility to extract second lowest byte of uint32_t.
inline uint8_t b2(uint32_t w) { return ((w >> 8) & 0xFF); }

/// Utility to extract third lowest byte of uint32_t.
inline uint8_t b3(uint32_t w) { return ((w >> 16) & 0xFF); }

/// Utility to extract high (forth lowest) byte of uint32_t.
inline uint8_t b4(uint32_t w) { return ((w >> 24) & 0xFF); }

/// Utility to pack four bytes into a uint32_t.
inline uint32_t bpack4(uint8_t b4, uint8_t b3, uint8_t b2, uint8_t b1) {
    return ((b4 << 24) | (b3 << 16) | (b2 << 8) | b1);
}

/// Utility to reverse a uint32_t from big endian to little endian or back.
inline uint32_t bflip4(uint32_t d) {
    return bpack4(b1(d),b2(d),b3(d),b4(d));
}

/// Utility to pack four bytes from array into a uint32_t.
/// lsb is first in the array
inline uint32_t stoi_l(uint8_t *s) {
    return ((s[3] << 24) | (s[2] << 16) | (s[1] << 8) | s[0]);
}

/// Utility to pack four bytes from array into a uint32_t.
/// msb is first in the array
inline uint32_t stoi_b(uint8_t *s) {
    return ((s[0] << 24) | (s[1] << 16) | (s[2] << 8) | s[3]);
}

/// Utility to pack four bytes from array into a uint16_t.
/// lsb is first in the array
inline uint16_t stoi16_l(uint8_t *s) {
    return ((s[1] << 8) | s[0]);
}

/// Utility to pack four bytes from array into a uint16_t.
/// msb is first in the array
inline uint16_t stoi16_h(uint8_t *s) {
    return ((s[0] << 8) | s[1]);
}

/// Utility converts ascii hex character to number.
inline int atoh_b(uint8_t s) {
    if(s >= '0' && s <= '9'){
        return s - '0';
    }
    if(s >= 'A' && s <= 'F'){
        return s - 'A' + 10;
    }
    if(s >= 'a' && s <= 'f'){
        return s - 'a' + 10;
    }
    return -1;
}

#endif
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
 * Standard system-wide errors (always use as negative -ERROR).
 *
 * @file    error.h
 */

#ifndef __ERROR_H__
#define __ERROR_H__

typedef int error_t;

/**
 * Defines general system errors.  Other modules may extend the error codes
 * by defining them in a specific range.  Errors below 1000 are reserved.
 * These enums are to be negated when used, thus code would look like this:
 *    return -EFAIL
 * Error codes are always negative with the exception of ESUCCESS, which is 0.
 * Positive values are reserved for valid return data as defined by the function.
 */
typedef enum {
  ESUCCESS       = 0,        ///< Everything is just fine

  EFAIL          = 1,        ///< General failure
  ENULL	         = 2,        ///< Unexpected NULL pointer detected
  EEBUSY         = 3,        ///< Not initialized fully, or not ready
  EINVALID       = 4,        ///< Invalid parameters passed
  ETIMEOUT       = 5,        ///< Ran out of time and gave up
  EBAD_CRC       = 6,        ///< CRC codes do not match
//  EOVERFLOW      = 7,        ///< Given buffer too small for data
  EINCOMPLETE    = 8,        ///< Not all expected data arrived
  EMEDIA         = 9,        ///< Storage media error
  EFILESYSTEM    = 10,       ///< Filesystem error
  ERESOURCE      = 11,       ///< Resource error
  EALLOC         = 12,       ///< Resource allocation
} err_sys_t;


/**
 * A macro designed to simplify running code and returning an error.
 * Assumes int err; defined.
 */
#define ERR_CHECK(x)    if (err=(x)) return err
#define ERR_CATCH(x)  { int err=(x); if ((err) < ESUCCESS) return (err); }

#endif

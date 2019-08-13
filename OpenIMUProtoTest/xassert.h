/** ***************************************************************************
 * @file   main.c
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
 * assert functionality implementation
 *
 * @file    xassert.h
 *
 */

#ifndef _XASSERT_H
#define _XASSERT_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PC_TEST
void OS_critical_enter(void);
void OS_delay(uint32_t delay);
#endif

#ifdef __cplusplus
}
#endif

#ifndef XASSERT_LEVEL
#define XASSERT_LEVEL 0
#endif

#ifndef USE_PRINTF
#define printf(...)
#endif

#define ASSERT(...) XASSERTF(__VA_ARGS__)

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------
#ifdef __RELEASE__
    // TODO maybe do some logging and reset here
    #define XASSERT(...)
    #define XASSERTF(...)
#elif defined (PC_TEST)
    #if (XASSERT_LEVEL == 0)
        /// Checks that the given condition is true,
        /// otherwise stops the program execution.
        /// \param condition  Condition to verify.
        #define XASSERT(condition)  { \
            if (!(condition)) { \
                while (1); \
            } \
        }

        #define XASSERTF(condition, ...)  { \
            if (!(condition)) { \
                printf("-F- ASSERT: "); \
                printf(__VA_ARGS__); \
                while (1); \
            } \
        }

    #else
        /// Checks that the given condition is true, otherwise
        /// stops the program execution.
        /// \param condition  Condition to verify.
        #define XASSERT(condition)  { \
            if (!(condition)) { \
                while (1); \
            } \
        }

        #define XASSERTF(condition, ...)  { \
            if (!(condition)) { \
                printf("-F- ASSERT: "); \
                printf(__VA_ARGS__); \
                while (1); \
            } \
        }
    #endif

#else

    #if (XASSERT_LEVEL == 0)
        /// Checks that the given condition is true,
        /// otherwise stops the program execution.
        /// \param condition  Condition to verify.
        #define XASSERT(condition)  { \
            if (!(condition)) { \
                OS_critical_enter(); \
                while (1); \
            } \
        }

        #define XASSERTF(condition, ...)  { \
            if (!(condition)) { \
                OS_critical_enter(); \
                printf("-F- ASSERT: "); \
                printf(__VA_ARGS__); \
                while (1); \
            } \
        }

    #else
        /// Checks that the given condition is true, otherwise
        /// stops the program execution.
        /// \param condition  Condition to verify.
        #define XASSERT(condition)  { \
            if (!(condition)) { \
                OS_critical_enter(); \
                while (1); \
            } \
        }

        #define XASSERTF(condition, ...)  { \
            if (!(condition)) { \
                OS_critical_enter(); \
                printf("-F- ASSERT: "); \
                printf(__VA_ARGS__); \
                while (1); \
            } \
        }

    #endif
#endif










#endif //#ifndef ASSERT_H


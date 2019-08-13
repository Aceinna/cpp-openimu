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
 * Implementation of os classes.
 *
 * @file    os.hpp
 */

#ifndef __OS_HPP__
#define __OS_HPP__
#include "string.h"
// ======== Include os-dependent headers ========

// ==== FreeRTOS
#ifdef __FREERTOS__
extern "C" {
#include "FreeRTOS.h"
#include "portmacro.h"
}
#include "os/freertos/Task_freertos.hpp"
#include "os/freertos/Semaphore_freertos.hpp"
#include "os/freertos/Systimer_freertos.hpp"
#include "os/freertos/Mutex_freertos.hpp"

// ==== NAVOS
#elif defined(__NAVOS__)
extern "C" {
#include "../os/navos/include/os.h"
}
#include "../os/navos/include/OsTask_navos.hpp"
#include "../os/navos/include/OsSemaphore_navos.hpp"
#include "../os/navos/include/OsMutex_navos.hpp"
#include "../os/navos/include/OsQueue_navos.hpp"


// Allows OS to override libcpu IRQ support with its own.
#define __OS_DEFINITIONS__
#define __OS_BOOT_IRQ_HANDLER     __weak
#define __OS_BOOT_IRQ_ENABLED     0

// ==== For tests that don't need to run the OS
#elif defined(PC_TEST)
#include "os_test.h"
#include "os_pc_queue.hpp"

// ==== Atomthreads
#elif defined(__ATOMTHREADS__)
extern "C" {
#include "atom.h"
#include "atomsem.h"
}
#include "os/atomthreads/Semaphore_atomthreads.hpp"

#elif defined(__OS_BAREMETAL__)
// ==== Bare Metal
//#define __OS_BAREMETAL__

#include "../os/baremetal/OsTask_baremetal.hpp"
#include "../os/baremetal/OsSemaphore_baremetal.hpp"
#include "../os/baremetal/OsMutex_baremetal.hpp"
#else

#include "OsTask_dummy.hpp"

#endif

// ==== Default definitions
// Use libcpu IRQ support by default.
#ifndef __OS_DEFINITIONS__
#define __OS_BOOT_IRQ_HANDLER
#define __OS_BOOT_IRQ_ENABLED    1
#endif

#define INFINITE_DELAY Task::DELAY_INFINITE
#define ZERO_DELAY     Task::DELAY_NONE


#endif

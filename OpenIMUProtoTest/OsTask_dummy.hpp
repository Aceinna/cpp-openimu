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
 * Abstract base class for OS tasks -- specifically for the case of no OS.
 *
 * @file    Task_Dummy.hpp
 */

 
#ifndef __I_TASK_DUMMY_HPP__
#define __I_TASK_DUMMY_HPP__

#include <stdint.h>

class Task {

public:

  enum{
  	DELAY_INFINITE,
  	DELAY_NONE
  };	

  static void yield()                {}
  static void schedulerInit()  		 {}
  static void schedulerStart() 	     {};
  static void schedulerStop()  	  	 {};
  static void enterCritical()  		 {};
  static void exitCritical()   		 {};
  static void delay(uint32_t delay)  {};
  static void sleep(uint32_t delay)  {};
  static uint32_t sysTime()          { return 0;}

};

#endif

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
 * C++ interface for peripheral devices.
 *
 * @file    IDevice.hpp
 */

#ifndef __IDEVICE_HPP__
#define __IDEVICE_HPP__

#include <stdint.h>
#include "error.h"


class IDevice {
    bool    _fOpened;
    bool    _isShareable;


public:
    IDevice(bool shareable = false) : _fOpened(false), _isShareable(shareable) {};
    /**
     * @returns true if device can be opened or shared
     */
    virtual bool    isAvailable(){
        if(!_fOpened || (_fOpened && _isShareable)){
            return true;
        }
        return false;
    }

    /**
     * Enables device. takes it out of lowpower mode and provides clock.
     * (if applicable).
     */
    virtual void    disable() = 0;
    /**
     * Disables device. Puts it into lowpower mode and turns clock off.
     * (if applicable).
     */
    virtual void    enable()  = 0;
    /**
     * Marks device as opened.
    * @return ESUCCESS if succeed
     */
    virtual error_t  markOpened()  {_fOpened = true; return ESUCCESS;}
    /**
     * Marks device as closed.
    * @return ESUCCESS if succeed
     */
    virtual error_t  markClosed()  {_fOpened = false; return ESUCCESS;}

};

#endif

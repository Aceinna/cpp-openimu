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

#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#define MATH_CONSTANTS_PI      3.1415926535897932384626433832795
#define MATH_CONSTANTS_G_FPS2  32.1740
#define MATH_CONSTANTS_G_MPS2  (MATH_CONSTANTS_G_FPS2 * 0.3048)

//#define MATH_CONSTANTS_G_MPS2  9.80665d0    //< average gravity at Earth's surface

#define UNIT_CONVERSIONS_DEG_TO_RAD  (MATH_CONSTANTS_PI / 180.0)
#define UNIT_CONVERSIONS_RAD_TO_DEG  (1.0 / UNIT_CONVERSIONS_DEG_TO_RAD)
#define UNIT_CONVERSIONS_FT_TO_M     (0.3048)
#define UNIT_CONVERSIONS_MI_TO_M     (1609.344)
#define UNIT_CONVERSIONS_NMI_TO_M    (1852.0)
#define UNIT_CONVERSIONS_M_TO_FT     (1.0 / UNIT_CONVERSIONS_FT_TO_M)
#define UNIT_CONVERSIONS_FPS_TO_MPS  (UNIT_CONVERSIONS_FT_TO_M)
#define UNIT_CONVERSIONS_MPH_TO_MPS  (0.44704)
#define UNIT_CONVERSIONS_KTS_TO_MPS  (0.514444)
#define UNIT_CONVERSIONS_MPS_TO_FPS  (1.0 / UNIT_CONVERSIONS_FPS_TO_MPS)


#define PI   MATH_CONSTANTS_PI
#define D2R  0.017453292519943    // degree to radian conversion
#define R2D  57.295779513082323
#define PI_OVER_2   1.570796326794897

#define Z_SCORE_95_CONFIDENCE       1.96  // value of the normal dist's z-score to capture 95% of the area


#endif

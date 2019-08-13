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
// main project file.

#include "stdafx.h"
#include <string.h>
#include "Form1.h"
#include "protocol.hpp"
using namespace ProbeCon;
using namespace System::Runtime::InteropServices;
using namespace System;




#define	 SERIAL_PORT_PREFFIX "\\\\.\\"

[STAThreadAttribute]

/*
void MarshalString (String ^ s, string & os ) {
   using namespace Runtime::InteropServices;
   const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
   os = chars;
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}
*/



int main(cli::array<System::String^> ^args)
{
	int portArray[256];
	int portCount = 255, res, count;
	char *name;
	String ^ss;

	TestProto	*protocol = new TestProto;


	/*
	count = args->Length;
	if(count){
		name = (char*)(Marshal::StringToHGlobalAnsi(args[0])).ToPointer();
	}else {
		return -1;
	}
*/

//	res = protocol->detect(name);
	memset(portArray, 0, sizeof(portArray));	
	res = protocol->detectComPorts(portArray, &portCount);
	if(res != 0){
		return -1;
	}

	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Form1^ form = gcnew Form1;
	if(form){
		form->setProtocol(protocol);
		form->setComPorts(portArray, portCount);

	}
	//	Application::Run(gcnew Form1());
	Application::Run(form);
	return 0;
}

uint32_t OS_get_time(void)
{
	return 0;
}

void OS_delay(uint32_t ms)
{
	Sleep(ms);
}

void OS_critical_enter()
{
}

void OS_critical_exit()
{
}


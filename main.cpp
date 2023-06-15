#pragma once
/**
 * program used for Watson Lab data collection via Labjack T7. 
**/

// For printf



//#include <stdio.h>

// For cin/cout
//#include <iostream>

// For csv files
//#include <fstream>
// For the LabJackM Library
//#include "LabJackM.h"

// For LabJackM helper functions, such as OpenOrDie, PrintDeviceInfoFromHandle,
// ErrorCheck, etc., such as OpenOrDie, PrintDeviceInfoFromHandle,
// ErrorCheck, etc.
 //#include "LJM_Utilities.h"

// For time
//#include <chrono>

#include "NoahLabjack.h"

int main()
{

	NoahLabjack david;

	david.start();

	
}
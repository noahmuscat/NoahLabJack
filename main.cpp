#pragma once


#include "NoahLabjack.h"

int main()
{
	// add loop that constantly checks for lights on/off ?
	// maybe a way to do a single while(1) loop that could create multiple labjacks --> reduce runtime and complication


	NoahLabjack david;
	
	if (david.triggerCheck()) {
		david.start();
	}
	
	
}
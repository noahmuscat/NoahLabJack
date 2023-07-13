#pragma once


#include "NoahLabjack.h"

int main()
{
	// add loop that constantly checks for lights on/off ?
	// also note, a value of 2.36 is the standard voltage that the constructor sets (after the great Lebron James), didn't want to use 0 or 1 to confuse

	int frames = 3; 
	const char* name[MAX_FRAMES] = {};

	name[0] = "AIN0";
	name[1] = "FIO0";
	name[2] = "FIO2";

	const char* trigChan = "FIO2"; 

	NoahLabjack josh(frames, name, trigChan);

	if (josh.triggerCheck()) {
		josh.start();
	}

	
	/* maybe a way to do a single while (1) loop that could create multiple labjacks--> reduce runtime and complication

	 - prompt for number of labjacks, etc.
	 - run while(1) loop (one iteration covers all desired labjacks, should still be able to have separate csvs, etc)
	 
	 only issue I can think of right now is the LJM_Open function and how to specify which labjack is which
	 just thought of another issue --> this would make the trigger channel very hard to implement (unless we could use just one trigger channel for all 4 labjacks)

	*/
	
	
}
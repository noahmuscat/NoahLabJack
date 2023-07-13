#include "NoahLabjack.h"
/**
 * program used for Watson Lab data collection via Labjack T7.
**/

// For printf
#include <stdio.h>

// For cin/cout
#include <iostream>

// For csv files
#include <fstream>
// For the LabJackM Library
#include "LabJackM.h"

// For LabJackM helper functions, such as OpenOrDie, PrintDeviceInfoFromHandle,
// ErrorCheck, etc., such as OpenOrDie, PrintDeviceInfoFromHandle,
// ErrorCheck, etc.
#include "LJM_Utilities.h"

// For time
#include <chrono>

NoahLabjack::NoahLabjack() {

	// Open first found LabJack
	err = LJM_Open(LJM_dtANY, LJM_ctANY, "LJM_idANY", &handle);
	ErrorCheck(err, "LJM_Open");

	deviceType = GetDeviceType(handle);
	msDelay = 100;
	numFrames = 1;
	
	for (int i = 0; i < numFrames; i++) {
		aNames[i] = " ";
		aValues[i] = 2.3;
	}
}

NoahLabjack::NoahLabjack(int frameNum, const char* names[MAX_FRAMES]) {

	// Open first found LabJack
	err = LJM_Open(LJM_dtANY, LJM_ctANY, "LJM_idANY", &handle);
	ErrorCheck(err, "LJM_Open");

	deviceType = GetDeviceType(handle);
	msDelay = 100;
	numFrames = frameNum;

	for (int i = 0; i < numFrames; i++) {
		aNames[i] = names[i];
		aValues[i] = 2.3;
	}
	// would probably format so that the trigger channel is input first, so we know which to assign as the trigger channel
}

bool NoahLabjack::triggerCheck() {

	int i;
	int errorAddress = INITIAL_ERR_ADDRESS;
	int skippedIntervals;
	const int INTERVAL_HANDLE = 1;

	PrintDeviceInfoFromHandle(handle);
	printf("\n");

	// Begin the loop
	printf("Starting loop.  Press Ctrl+c to stop.\n\n");
	// The LabJackM (LJM) library will catch the Ctrl+c signal, close
	// all open devices, then exit the program.

	err = LJM_StartInterval(INTERVAL_HANDLE, msDelay * 1000);
	ErrorCheck(err, "LJM_StartInterval");\

	// keeps track of previous values (either 0 or 1)
	char ctrlVar = ' ';

	while (1) {
		// configures one port for the trigger check
		aNames[0] = "FIO2";
		// reads in values
		err = LJM_eReadNames(handle, numFrames, (const char**)aNames, aValues,
			&errorAddress);
		ErrorCheckWithAddress(err, errorAddress, "LJM_eReadNames");

		// prints values to command line and to csv
		printf("eReadNames  :");
		for (i = 0; i < numFrames; i++) {
			printf(" %s = %.4f  ", aNames[i], aValues[i]);
		}
		printf("\n");

		// waiting for next iteration of data
		err = LJM_WaitForNextInterval(INTERVAL_HANDLE, &skippedIntervals);
		ErrorCheck(err, "LJM_WaitForNextInterval");
		if (skippedIntervals > 0) {
			printf("SkippedIntervals: %d\n", skippedIntervals);
		}
		printf("\n");


		if (aValues[0] == 1.0) {
			ctrlVar = aValues[0];
		}

		if (aValues[0] == 0.0 && ctrlVar == 1.0) {
			return true;
		}
	}

	// Close interval and device handles
	err = LJM_CleanInterval(INTERVAL_HANDLE);
	ErrorCheck(err, "LJM_CleanInterval");

	err = LJM_Close(handle);
	ErrorCheck(err, "LJM_Close");

	WaitForUserIfWindows();

	return false;

}

void NoahLabjack::start() {
	int i;
	int errorAddress = INITIAL_ERR_ADDRESS;
	int skippedIntervals;
	const int INTERVAL_HANDLE = 1;

	PrintDeviceInfoFromHandle(handle);
	printf("\n");

	deviceType = GetDeviceType(handle);

	uint64_t timeSinceEpochMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


	// eventually create separate csv files for analog and digital info

	std::ofstream dataFile("data.csv");
	// Headers
	dataFile << "ComputerTime" << "," << "AIN0" << "," << "FIO0" << "," << "FIO2" << std::endl;


	// Begin the loop
	printf("Starting loop.  Press Ctrl+c to stop.\n\n");
	// The LabJackM (LJM) library will catch the Ctrl+c signal, close
	// all open devices, then exit the program.

	err = LJM_StartInterval(INTERVAL_HANDLE, msDelay * 1000);
	ErrorCheck(err, "LJM_StartInterval");

	while (1) {
		// configures three ports
		numFrames = 3;
		aNames[0] = "AIN0";
		aNames[1] = "FIO0";
		aNames[2] = "FIO2";
		// reads in values
		err = LJM_eReadNames(handle, numFrames, (const char**)aNames, aValues,
			&errorAddress);
		ErrorCheckWithAddress(err, errorAddress, "LJM_eReadNames");

		// prints values to command line and to csv
		printf("eReadNames  :");
		for (i = 0; i < numFrames; i++) {
			printf(" %s = %.4f  ", aNames[i], aValues[i]);
		}
		printf("\n");

		// for csv file
		dataFile << timeSinceEpochMilliseconds << "," << aValues[0] << "," << aValues[1] << "," << aValues[2] << std::endl;

		// waiting for next iteration of data
		err = LJM_WaitForNextInterval(INTERVAL_HANDLE, &skippedIntervals);
		ErrorCheck(err, "LJM_WaitForNextInterval");
		if (skippedIntervals > 0) {
			printf("SkippedIntervals: %d\n", skippedIntervals);
		}
		printf("\n");
	}

	// Close interval and device handles
	err = LJM_CleanInterval(INTERVAL_HANDLE);
	ErrorCheck(err, "LJM_CleanInterval");

	err = LJM_Close(handle);
	ErrorCheck(err, "LJM_Close");

	WaitForUserIfWindows();

	dataFile.close();

}

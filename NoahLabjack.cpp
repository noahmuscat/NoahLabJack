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


int NoahLabjack::start() {
	int err;
	int handle;
	int i;
	int errorAddress = INITIAL_ERR_ADDRESS;
	int skippedIntervals;
	int deviceType;

	const int INTERVAL_HANDLE = 1;
	int it = 0;

	// interval time (ms)
	int msDelay = 100;

	// hopefully 10 ports is fine
	enum { MAX_FRAMES = 10 };

	int numFrames;
	const char* aNames[MAX_FRAMES];
	double aValues[MAX_FRAMES];

	uint64_t timeSinceEpochMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	// Open first found LabJack
	err = LJM_Open(LJM_dtANY, LJM_ctANY, "LJM_idANY", &handle);
	ErrorCheck(err, "LJM_Open");

	PrintDeviceInfoFromHandle(handle);
	printf("\n");

	deviceType = GetDeviceType(handle);

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

		++it;

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

	return LJME_NOERROR;
}
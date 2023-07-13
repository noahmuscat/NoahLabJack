#pragma once
/**
 * program used for Watson Lab data collection via Labjack T7.
**/

enum { MAX_FRAMES = 10 };
#define INITIAL_ERR_ADDRESS -2;


class NoahLabjack
{
public:
	//NoahLabjack();
	NoahLabjack(int frameNum, const char* names[MAX_FRAMES], const char* trigChannel);

	bool triggerCheck();
	void start();

private:
	int err;
	int handle;
	int deviceType;
	int msDelay;
	int numFrames;
	const char* aNames[MAX_FRAMES];
	double aValues[MAX_FRAMES];

	// temporary stuff all for the trigger channel
	const char* tempNames[MAX_FRAMES];
	const char* triggerChannel;
	int tempNumFrames;
};


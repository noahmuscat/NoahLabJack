#pragma once
/**
 * program used for Watson Lab data collection via Labjack T7.
**/

enum { MAX_FRAMES = 10 };
#define INITIAL_ERR_ADDRESS -2;


class NoahLabjack
{
public:
	NoahLabjack();
	NoahLabjack(int frameNum, const char* names[MAX_FRAMES]);

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
};


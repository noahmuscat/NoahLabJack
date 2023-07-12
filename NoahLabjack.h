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

	bool triggerCheck();
	void start();

	int err;
	int handle;
};


#include "DeltaTimer.h"

DeltaTimer & pxpk::DeltaTimer::getInstance()
{
	static DeltaTimer instance;
	return instance;
}

pxpk::DeltaTimer::~DeltaTimer()
{
}

void pxpk::DeltaTimer::tick()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = std::chrono::high_resolution_clock::now();
	
	// reset timer if first run
	if (firstRun)
	{
		frames = 0;
		lastTickTime = currentTime;
		firstRun = false;
		return;
	}

	//increment frames
	frames++;

	// calculate deltaTime


	// if more than 0.25 seconds have passed, calculate FPS
}

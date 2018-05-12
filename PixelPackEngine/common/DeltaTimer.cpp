#include "DeltaTimer.h"

pxpk::DeltaTimer::~DeltaTimer()
{
}

long pxpk::DeltaTimer::getFrameTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
}

long pxpk::DeltaTimer::getTimeStep()
{
	return timeStep.count();
}

void pxpk::DeltaTimer::setTimeStep(long input)
{
	timeStep = std::chrono::milliseconds{ input };
}

bool pxpk::DeltaTimer::tickCheckUpdate()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = std::chrono::high_resolution_clock::now();
	
	bool timeToUpdate = false;

	// reset timer if first run
	if (firstRun)
	{
		lastTickTime = currentTime;
		lastUpdateTime = currentTime;
		firstRun = false;
		return;
	}

	// calculate frameTime
	frameTime = currentTime - lastTickTime;

	// reset lastTickTime
	lastTickTime = currentTime;

	//check if an update is needed
	if (currentTime - lastUpdateTime > timeStep)
	{
		timeToUpdate = true;
		lastUpdateTime += timeStep;
	}

	return timeToUpdate;
}

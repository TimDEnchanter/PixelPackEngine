#ifndef DELTA_TIMER_H
#define DELTA_TIMER_H

#include <chrono>

namespace pxpk
{
	class DeltaTimer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> lastTickTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> lastUpdateTime;
		std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> frameTime;
		std::chrono::milliseconds timeStep;

		bool firstRun;

	public:
		DeltaTimer() :
			lastTickTime(std::chrono::high_resolution_clock::now()),
			frameTime(std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period>::zero()),
			timeStep(250),
			firstRun(true)
		{};

		~DeltaTimer();

		long getFrameTime();
		long getTimeStep();

		void setTimeStep(long);

		bool tickCheckUpdate();
	};
}

#endif // DELTA_TIMER_H

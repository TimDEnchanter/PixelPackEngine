#ifndef DELTA_TIMER_H
#define DELTA_TIMER_H

#include <chrono>

namespace pxpk
{
	class DeltaTimer
	{
	private:
		DeltaTimer() :
			lastTickTime(std::chrono::high_resolution_clock::now()),
			deltaTime(0.0),
			frames(0),
			fps(0.0f),
			firstRun(true)
			{};

		std::chrono::time_point<std::chrono::high_resolution_clock> lastTickTime;
		long deltaTime;
		int frames;
		float fps;
		bool firstRun;

	public:
		static DeltaTimer& getInstance();

		~DeltaTimer();

		DeltaTimer(DeltaTimer const &) = delete;
		void operator=(DeltaTimer const &) = delete;

		void tick();

		long getDeltaTime();
		float getFPS();
	};
}

#endif // DELTA_TIMER_H

#include "GameTime.h"
#include <chrono>
#include <chrono>
#include <chrono>
#include <chrono>
#include <chrono>
#include <chrono>

namespace GameEngine
{
	__ImplementClass(GameTime, 'GEGT', Core::RefCounted)
	__ImplementSingleton(GameTime)
	
	GameTime::GameTime() : deltaTime(0), fixedDeltaTime(0), ticks(0), timeScale(1), count(0), frames(0), lastFrameTick(0)
	{
		__ConstructSingleton
		const clock_t timerStart = clock();
	}

	GameTime::~GameTime()
	{
		__DestructSingleton
	}

	void GameTime::Update()
	{
		const auto current = clock();
		ticks = float(current - timerStart) / CLOCKS_PER_SEC;
		ticks *= 1000;
		
		deltaTime = fixedDeltaTime = (ticks - lastFrameTick) / 1000;
		deltaTime *= timeScale;

		count += 1;
		frames += 1;
		
		lastFrameTick = ticks;
	}

	std::chrono::steady_clock::time_point GameTime::TimerStart()
	{
		return std::chrono::steady_clock::now();
	}

	double GameTime::TimerStop(const std::chrono::steady_clock::time_point& start) const
	{
		return std::chrono::duration_cast<std::chrono::duration<double>>(Clock::now() - start).count();
	}

	int GameTime::TimerResult() const
	{
		return frames;
	}
}

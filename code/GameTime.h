#pragma once
#include "stdneb.h"
#include "core/refcounted.h"
#include "core/singleton.h"
#include <chrono>

namespace GameEngine
{
	class GameTime final : public Core::RefCounted
	{
		__DeclareClass(GameTime)
		__DeclareSingleton(GameTime)
		
	public:
		double deltaTime;
		double fixedDeltaTime;
		int timeScale;
		float ticks;

		GameTime();
		~GameTime();
		
		void Update();

		/* Framecounter */
		std::chrono::steady_clock::time_point TimerStart();
		double TimerStop(const std::chrono::steady_clock::time_point& start) const;
		int TimerResult() const;
		
	private:
		typedef std::chrono::high_resolution_clock Clock;
		clock_t timerStart;
		int count;
		int frames;
		float lastFrameTick;
	};
}

#pragma once
#include "stdneb.h"
#include "core/refcounted.h"
#include "core/singleton.h"

namespace GameEngine
{
	class GameTime final : public Core::RefCounted
	{
		__DeclareClass(GameTime)
		__DeclareSingleton(GameTime)
	public:
		double deltaTime;
		double fixedDeltaTime;
		float ticks;
		float timeScale;
		
		GameTime();
		~GameTime();
		
		void Update();

		/* Framecounter */
		void TimerStart();
		void TimerStop();
		int TimerResult() const;
		
	private:
		long count;
		int frames;
		int lastFrameTick;
	};
}

#pragma once
#include "stdneb.h"
#include "core/refcounted.h"
#include "core/singleton.h"

namespace GameEngine
{
	class GameTime final : public Core::RefCounted
	{
		__DeclareSingleton(GameEngine::GameTime)
		
	public:

		GameTime() : count(0), frames(0) { }

		void Update();

		// basic framecounter
		void StartTimer();
		void StopTimer();
		int GetFramesElapsed() const;
		
	private:
		long count;
		int frames;
		
	};
}

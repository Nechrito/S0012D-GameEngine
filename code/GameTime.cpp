#include "GameTime.h"

namespace GameEngine
{
	__ImplementSingleton(GameEngine::GameTime);
	
	void GameTime::Update()
	{
		count += 1;
		frames += 1;
	}

	void GameTime::StartTimer()
	{
		frames = 0;
	}

	void GameTime::StopTimer()
	{
		count = 0;
	}

	int GameTime::GetFramesElapsed() const
	{
		return frames;
	}
}

#include "GameTime.h"

namespace GameEngine
{
	__ImplementClass(GameTime, 'GEGT', Core::RefCounted)
	__ImplementSingleton(GameTime)
	
	GameTime::GameTime() : deltaTime(0), fixedDeltaTime(0), ticks(0), timeScale(1), count(0), frames(0), lastFrameTick(0)
	{
		__ConstructSingleton
	}

	GameTime::~GameTime()
	{
		__DestructSingleton
	}

	void GameTime::Update()
	{
		count += 1;
		frames += 1;

		ticks = GetTickCount();
		
		fixedDeltaTime = (ticks - lastFrameTick) / 1000;
		deltaTime = fixedDeltaTime * timeScale;

		lastFrameTick = ticks;
	}

	void GameTime::TimerStart()
	{
		frames = 0;
	}

	void GameTime::TimerStop()
	{
		count = 0;
	}

	int GameTime::TimerResult() const
	{
		return frames;
	}
}

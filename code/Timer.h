#pragma once
#include <chrono>

namespace GameEngine
{
	class Timer
	{
	public:
		static std::chrono::steady_clock::time_point CurrentTime()
		{
			return std::chrono::steady_clock::now();
		}

		static double TimeSince(const std::chrono::steady_clock::time_point start)
		{
			return std::chrono::duration_cast<std::chrono::duration<double>>(CurrentTime() - start).count();
		}
	};
}

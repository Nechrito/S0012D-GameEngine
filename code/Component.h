#pragma once

// Forward declaration avoids the parsing the compiler would have to do during an #include "..."
// A better optimization would ofc be pre-compiled headers
struct Telegram;

namespace GameEngine
{
	class Component
	{
	public:
		Component() = default;

		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
		virtual bool HandleMessage(const Telegram& msg) = 0;
		
		virtual ~Component() = default;
	};
}


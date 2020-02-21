#pragma once

//#include "util/queue.h"
#include "core/types.h"
#include "core/singleton.h"
#include "Telegram.h"

namespace GameEngine
{
	
	class BaseEntity;

	class MessageDispatcher
	{
		__DeclareSingleton(MessageDispatcher);
		
	public:

		void DispatchMessage(double delay, int sender, int receiver, int msg);
		void DispatchDelayedMessages();
		
	private:
		//Queue<Telegram> PriorityQueue;
		
		void Discharge(BaseEntity* receiver, const Telegram& msg);
	};
}


#pragma once
#include "stdneb.h"
#include "util/queue.h"
#include "Telegram.h"
#include "core/refcounted.h"
#include "core/singleton.h"

namespace GameEngine
{
	class BaseEntity;

	class MessageDispatcher : public Core::RefCounted
	{
		__DeclareSingleton(GameEngine::MessageDispatcher)
		
	public:

		void SendMessage(double delay, int sender, int receiver, int msg);
		void DispatchDelayedMessages();
		
	private:
		Util::Queue<Telegram> PriorityQueue;
		void Discharge(BaseEntity* receiver, const Telegram& msg);
	};
}


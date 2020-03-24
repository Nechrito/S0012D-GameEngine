#include "stdneb.h"
#include "MessageDispatcher.h"
#include "BaseEntity.h"
#include "EntityManager.h"
#include "GameTime.h"

namespace GameEngine
{
	__ImplementClass(MessageDispatcher, 'GEMD', Core::RefCounted)
	__ImplementSingleton(MessageDispatcher)

	MessageDispatcher::MessageDispatcher()
	{
		__ConstructSingleton
	}

	MessageDispatcher::~MessageDispatcher()
	{
		__DestructSingleton
	}

	void MessageDispatcher::SendMessage(double delay, int sender, int receiver, int msg)
	{
		BaseEntity* entity = EntityManager::Singleton->GetEntity(receiver);

		if (entity == nullptr)
		{
			// todo: log this
			return;
		}

		Telegram telegram(0, sender, receiver, msg);

		if (delay <= 0)
		{
			Discharge(entity, telegram);
		}
		else
		{
			const double currentTime = GameTime::Singleton->TimerResult();
			telegram.DispatchTime = currentTime + delay;

			// Add the telegram to the queue
			PriorityQueue.Enqueue(telegram);
		}
	}


	void MessageDispatcher::DispatchDelayedMessages()
	{
		const double currentTime = GameTime::Singleton->TimerResult();

		while (!PriorityQueue.IsEmpty())
		{
			const Telegram temp = PriorityQueue.Peek();
			if (temp.DispatchTime > currentTime || temp.DispatchTime <= 0)
			{
				break;
			}

			Telegram telegram = PriorityQueue.Dequeue();
			BaseEntity* receiver = EntityManager::Singleton->GetEntity(telegram.Receiver);
			Discharge(receiver, telegram);
		}
	}
	
	void MessageDispatcher::Discharge(BaseEntity* receiver, const Telegram& msg)
	{
		receiver->HandleMessage(msg);
	}
}

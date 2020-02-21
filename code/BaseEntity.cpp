#include "BaseEntity.h"
#include <cassert>
#include "Telegram.h"

namespace GameEngine
{
	__ImplementClass(GameEngine::BaseEntity, 'GEBE', Core::RefCounted);

	int BaseEntity::NextID = 0;

	BaseEntity::BaseEntity()
	{
		UniqueID = NextID;
		NextID = UniqueID + 1;
	}

	void BaseEntity::HandleMessage(const Telegram& msg)
	{
		for (Component* component : Components)
		{
			component->HandleMessage(msg);
		}
	}

	void BaseEntity::Init()
	{
		for (Component* component : Components)
		{
			component->Init();
		}
	}

	void BaseEntity::Update()
	{
		for (Component* component : Components)
		{
			component->Update();
		}
	}

	void BaseEntity::Shutdown()
	{
		for (Component* component : Components)
		{
			component->Shutdown();
		}
	}
}

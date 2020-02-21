#include "EntityManager.h"
#include "BaseEntity.h"

namespace GameEngine
{
	__ImplementSingleton(GameEngine::EntityManager);
	
	void EntityManager::RegisterEntity(BaseEntity* entity)
	{

	}

	void EntityManager::RemoveEntity(BaseEntity* entity)
	{

	}

	BaseEntity* EntityManager::GetEntity(int id)
	{
		return nullptr;
	}

	void EntityManager::Init()
	{
		for (BaseEntity* entity : Entities)
		{
			entity->Init();
		}
	}

	void EntityManager::Update()
	{
		for (BaseEntity* entity : Entities)
		{
			entity->Update();
		}
	}

	void EntityManager::Shutdown()
	{
		for (BaseEntity* entity : Entities)
		{
			entity->Shutdown();
		}
	}
}


#include "EntityManager.h"
#include "BaseEntity.h"
#include "TransformComponent.h"
#include "GraphicsComponent.h"

namespace GameEngine
{
	__ImplementClass(EntityManager, 'GEEM', Core::RefCounted)
	__ImplementSingleton(EntityManager)

	EntityManager::EntityManager()
	{
		__ConstructSingleton
	}

	EntityManager::~EntityManager()
	{
		__DestructSingleton
	}

	void EntityManager::RegisterEntity(BaseEntity* entity)
	{
		Entities.Append(entity);
		EntityTable.Add(entity->Name, Entities.Size());
	}

	BaseEntity* EntityManager::RegisterEntity(const Util::StringAtom& name, const Resources::ResourceName& uri, const Util::StringAtom& tag, const Math::point& position)
	{
		BaseEntity* entity = BaseEntity::Create();
		entity->Name = name;

		TransformComponent* transform = TransformComponent::Create();
		transform->SetOwner(entity);
		transform->Init();
		transform->SetPosition(position);

		GraphicsComponent* graphics = GraphicsComponent::Create();
		graphics->SetOwner(entity);
		graphics->Init();
		graphics->Load(uri, tag);
		
		entity->RegisterComponent(transform);
		entity->RegisterComponent(graphics);

		Entities.Append(entity);
		EntityTable.Add(name, Entities.Size());

		return entity;
	}

	void EntityManager::RemoveEntity(BaseEntity* entity)
	{
		const IndexT i = EntityTable[entity->Name];
		EntityTable.Erase(entity->Name);
		Entities.EraseIndexSwap(i);
	}

	BaseEntity* EntityManager::GetEntity(const Util::StringAtom& name)
	{
		for (auto& entity : Entities)
		{
			if (entity->Name == name)
			{
				return entity;
			}
		}
		
		//if (Entities.Size() > 0 && EntityTable.Size() > 0)
			//return Entities[EntityTable[name]];
		return nullptr;
	}

	BaseEntity* EntityManager::GetEntity(int id)
	{
		for (auto entity : Entities)
		{
			if (entity->UniqueID == id)
			{
				return entity;
			}
		}
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


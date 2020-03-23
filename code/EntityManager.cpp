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
		entities.Append(entity);
		entityTable.Add(entity->Name, entities.Size());
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

		const size_t index = entities.Size();
		
		entities.Append(entity);
		entityTable.Add(name, index);

		return entity;
	}

	void EntityManager::RemoveEntity(BaseEntity* entity)
	{
		const IndexT i = entityTable[entity->Name];
		entityTable.Erase(entity->Name);
		entities.EraseIndexSwap(i);
	}

	BaseEntity* EntityManager::GetEntity(const Util::StringAtom& name)
	{
		try
		{
			return entities[entityTable[name]];
		}
		catch (...)
		{
			n_printf("[Entity] Failed to get %s", name.Value());
			return nullptr;
		}
	}

	BaseEntity* EntityManager::GetEntity(int id)
	{
		// todo: implement 'IndexT HashCode() const' for O(1) lookup time 
		for (auto entity : entities)
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
		for (BaseEntity*& entity : entities)
		{
			if (entity)
				entity->Init();
		}
	}

	void EntityManager::Update()
	{
		for (BaseEntity*& entity : entities)
		{
			if (entity)
				entity->Update();
		}
	}

	void EntityManager::Shutdown()
	{
		for (BaseEntity*& entity : entities)
		{
			if (entity)
				entity->Shutdown();
		}
	}
}


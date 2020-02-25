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

	void EntityManager::RegisterEntity(const Util::StringAtom& name, const Resources::ResourceName& uri, const Util::StringAtom& tag, const Math::point& position)
	{
		BaseEntity* entity = BaseEntity::Create();
		entity->Name = name;

		TransformComponent* transform = TransformComponent::Create();
		transform->SetKey("Transform");
		transform->SetOwner(entity);
		
		GraphicsComponent* graphics = GraphicsComponent::Create();
		graphics->SetKey("Graphics");
		graphics->SetOwner(entity);

		transform->SetPosition(position);
		graphics->Load(uri, tag);
		
		entity->RegisterComponent(transform);
		entity->RegisterComponent(graphics);

		Entities.Append(entity);
		EntityTable.Add(name, Entities.Size());

		entity->Init();
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


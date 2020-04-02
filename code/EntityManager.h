#pragma once
#include "MessageDispatcher.h"
#include "util/stringatom.h"
#include "resources/resource.h"


namespace GameEngine
{
	class EntityManager final : public Core::RefCounted
	{
		__DeclareClass(EntityManager)
		__DeclareSingleton(EntityManager)

	public:
		EntityManager();
		~EntityManager();
		
		void RegisterEntity(BaseEntity* entity);
		BaseEntity* RegisterEntity(const Util::StringAtom& name, const Resources::ResourceName& uri, const Util::StringAtom& tag, const Math::point& position = Math::point());

		void RemoveEntity(BaseEntity* entity);
		
		BaseEntity* GetEntity(const Util::StringAtom& name);
		BaseEntity* GetEntity(int id);
		Util::Array<BaseEntity*> GetAllEntities() const;
		
		void Init();
		void Update();
		void Shutdown();
		
	private:
		Util::Array<BaseEntity*> entities;
		Util::HashTable<Util::StringAtom, IndexT> entityTable;
	};
}


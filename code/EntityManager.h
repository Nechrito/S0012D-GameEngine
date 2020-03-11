#pragma once
#include "stdneb.h"
#include "MessageDispatcher.h"
#include "util/stringatom.h"
#include "resources/resource.h"


namespace GameEngine
{
	class EntityManager : public Core::RefCounted
	{
		__DeclareClass(EntityManager)
		__DeclareSingleton(EntityManager)

	public:
		EntityManager();
		~EntityManager();
		
		void RegisterEntity(BaseEntity* entity);
		void RegisterEntity(const Util::StringAtom& name, const Resources::ResourceName& uri, const Util::StringAtom& tag, const Math::point& position = Math::point());

		void RemoveEntity(BaseEntity* entity);
		BaseEntity* GetEntity(const Util::StringAtom& name);
		BaseEntity* GetEntity(int id);

		void Init();
		void Update();
		void Shutdown();
		
	private:
		Util::Array<BaseEntity*> Entities;
		Util::HashTable<Util::StringAtom, IndexT> EntityTable;
	};
}


#pragma once
//#include "util/array.h"
//#include "util/queue.h"
//#include "util/variant.h"
//#include "util/stringatom.h"
//#include "Component.h"
#include "MessageDispatcher.h"


namespace GameEngine
{
	class EntityManager : public MessageDispatcher
	{
		__DeclareSingleton(EntityManager);

	public:
		void RegisterEntity(BaseEntity* entity);
		void RemoveEntity(BaseEntity* entity);
		BaseEntity* GetEntity(int id);

		void Init();
		void Update();
		void Shutdown();
		
	private:
		//Util::Array<BaseEntity*> Entities;
		//Util::HashTable<Util::StringAtom, IndexT> EntityTable;
	};
}


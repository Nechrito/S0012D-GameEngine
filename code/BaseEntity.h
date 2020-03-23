#pragma once
#include "stdneb.h"
#include "core/refcounted.h"
#include "util/variant.h"
#include "util/stringatom.h"
#include "Component.h"

struct Telegram;

namespace GameEngine
{
	class BaseEntity final : public Core::RefCounted
	{
		__DeclareClass(GameEngine::BaseEntity)
		
	public:
		static int NextID;
		int UniqueID;
		Util::StringAtom Name;

		BaseEntity();

		bool RegisterVariable(const Util::StringAtom& name, const Util::Variant& variable, bool overrideExisting = false);
		bool RegisterComponent(Component* component);

		Component* GetComponent(const Util::StringAtom& name);
		
		void HandleMessage(const Telegram& msg);
		
		void Init();
		void Update();
		void Shutdown();

	private:
		
		Util::Array<Component*> components;
		Util::HashTable<Util::StringAtom, IndexT> componentsTable;
		Util::HashTable<Util::StringAtom, Util::Variant> variables;
	};
}

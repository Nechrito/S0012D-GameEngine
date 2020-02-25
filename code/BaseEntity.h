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
		Util::Array<Component*> Components;
		Util::HashTable<Util::StringAtom, IndexT> ComponentsHash;

		Util::HashTable<Util::StringAtom, Util::Variant> Variables;

		BaseEntity();

		bool RegisterVariable(const Util::StringAtom& name, Util::Variant variable, bool overrideExisting = false);
		bool RegisterComponent(Component* component);

		Component* GetComponent(Util::StringAtom name);
		
		void HandleMessage(const Telegram& msg);
		
		void Init();
		void Update();
		void Shutdown();

	private:
#define Nameof(x) #x
		
	};
}

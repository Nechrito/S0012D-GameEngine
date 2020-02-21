#pragma once
#include "stdneb.h"
#include "core/refcounted.h"
#include "util/variant.h"
#include "util/stringatom.h"
#include "game/component/component.h"
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
		
		BaseEntity();

		void HandleMessage(const Telegram& msg);

		void Init();
		void Update();
		void Shutdown();

	protected:
		Util::StringAtom Name;
		Util::Array<Component*> Components;
		Util::HashTable<Util::StringAtom, Util::Variant> Variables;
	};
}

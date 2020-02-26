#include "BaseEntity.h"
#include <cassert>
#include "Telegram.h"

namespace GameEngine
{
	
	__ImplementClass(GameEngine::BaseEntity, 'GEBE', Core::RefCounted)

	int BaseEntity::NextID = 0;

	BaseEntity::BaseEntity()
	{
		UniqueID = NextID;
		NextID = UniqueID + 1;
	}

	bool BaseEntity::RegisterVariable(const Util::StringAtom& name, const Util::Variant variable, const bool overrideExisting)
	{
		if (Variables.Contains(name))
		{
			if (!overrideExisting)
				return false;

			Variables[name] = variable;
		}
		
		Variables.Add(name, variable);

		return true;
	}

	bool BaseEntity::RegisterComponent(Component* component)
	{
		const auto existingComponent = Components.Find(component);
		
		if (existingComponent != nullptr)
		{
			return false;
		}

		component->SetOwner(this);

		Components.Append(component);
		ComponentsHash.Add(component->GetName(), Components.Size());
		
		return true;
	}

	Component* BaseEntity::GetComponent(Util::StringAtom name)
	{
		if (Components.Size() > 0 && ComponentsHash.Size() > 0)
			return Components[ComponentsHash[name]];
		return nullptr;
	}

	void BaseEntity::HandleMessage(const Telegram& msg)
	{
		for (Component* component : Components)
		{
			component->HandleMessage(msg);
		}
	}

	void BaseEntity::Init()
	{
		for (Component* component : Components)
		{
			component->Init();
		}
	}

	void BaseEntity::Update()
	{
		for (Component* component : Components)
		{
			component->Update();
		}
	}

	void BaseEntity::Shutdown()
	{
		for (Component* component : Components)
		{
			component->Shutdown();
		}
	}
}

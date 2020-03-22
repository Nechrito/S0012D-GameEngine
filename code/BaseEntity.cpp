#include "BaseEntity.h"
#include <cassert>
#include "Telegram.h"
#include <stdexcept>

namespace GameEngine
{
	__ImplementClass(BaseEntity, 'GEBE', Core::RefCounted)

	int BaseEntity::NextID = 0;

	BaseEntity::BaseEntity()
	{
		UniqueID = NextID;
		NextID = UniqueID + 1;
	}

	bool BaseEntity::RegisterVariable(const Util::StringAtom& name, const Util::Variant& variable, const bool overrideExisting)
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
		
		if (component == nullptr || existingComponent != nullptr)
		{
			return false;
		}

		if (!ComponentsHash.Contains(component->GetType()))
			ComponentsHash.Add(component->GetType(), Components.Size());

		Components.Append(component);

		return true;
	}

	Component* BaseEntity::GetComponent(const Util::StringAtom& name)
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
		for (auto& component : Components)
		{
			if (component)
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

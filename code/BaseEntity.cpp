#include "stdneb.h"
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
		if (variables.Contains(name))
		{
			if (!overrideExisting)
				return false;

			variables[name] = variable;
		}
		
		variables.Add(name, variable);

		return true;
	}

	bool BaseEntity::RegisterComponent(Component* component)
	{
		const auto existingComponent = components.Find(component);
		
		if (component == nullptr || existingComponent != nullptr)
		{
			return false;
		}

		if (!componentsTable.Contains(component->GetIdentifier()))
			componentsTable.Add(component->GetIdentifier(), components.Size());

		components.Append(component);

		return true;
	}

	Component* BaseEntity::GetComponent(const Util::StringAtom& name)
	{
		if (components.Size() > 0)
		{
			try
			{
				return components[componentsTable[name]];
			}
			catch (...)
			{
				n_printf("[Component] Failed to get %s", name.Value());
			}
		}
		
		return nullptr;
	}

	void BaseEntity::HandleMessage(const Telegram& msg)
	{
		for (auto& component : components)
		{
			component->HandleMessage(msg);
		}
	}

	void BaseEntity::Init()
	{
		for (auto& component : components)
		{
			if (component)
				component->Init();
		}
	}

	void BaseEntity::Update()
	{
		for (auto& component : components)
		{
			component->Update();
		}
	}

	void BaseEntity::Shutdown()
	{
		for (auto& component : components)
		{
			component->Shutdown();
		}
	}
}

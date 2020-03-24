#include "stdneb.h"
#include "GraphicsComponent.h"
#include "graphics/graphicsserver.h"
#include "models/modelcontext.h"
#include "visibility/visibilitycontext.h"
#include "BaseEntity.h"
#include "TransformComponent.h"

namespace GameEngine
{
	__ImplementClass(GraphicsComponent, 'GEGC', Core::RefCounted)

	void GraphicsComponent::Load(const Resources::ResourceName& resource, const Util::StringAtom& tag)
	{
		Models::ModelContext::Setup(ID, resource, tag);
		Visibility::ObservableContext::Setup(ID, Visibility::Model);
		
		if (Owner != nullptr)
		{
			TransformComponent* entityTransform = dynamic_cast<TransformComponent*>(Owner->GetComponent("Transform"));
			if (entityTransform)
				Models::ModelContext::SetTransform(ID, entityTransform->Transform.getmatrix());
		}
	}

	void GraphicsComponent::Init()
	{
		if (Initialized)
			return;
		
		this->SetType("Graphics");
		ID = Graphics::CreateEntity();
		Graphics::RegisterEntity<Models::ModelContext, Visibility::ObservableContext>(ID);
	}

	void GraphicsComponent::Update()
	{
		if (Owner != nullptr)
		{
			TransformComponent* entityTransform = dynamic_cast<TransformComponent*>(Owner->GetComponent("Transform"));
			if (entityTransform)
				Models::ModelContext::SetTransform(ID, entityTransform->Transform.getmatrix());
		}
	}

	void GraphicsComponent::Shutdown()
	{
		Graphics::DeregisterEntity<Models::ModelContext, Visibility::ObservableContext>(ID);
		Graphics::DestroyEntity(ID);
	}

	bool GraphicsComponent::HandleMessage(const Telegram& msg)
	{
		return true;
	}
}


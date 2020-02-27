#include "TransformComponent.h"
#include "BaseEntity.h"
#include "GameTime.h"

namespace GameEngine
{
	__ImplementClass(GameEngine::TransformComponent, 'GETC', Core::RefCounted)

	void TransformComponent::Init()
	{
		if (Initialized)
			return;

		this->SetType("Transform");
		Velocity = Math::float4(0);
		Owner->RegisterVariable("Velocity", Velocity);
	}

	void TransformComponent::Update()
	{
		const Math::point current = Transform.getposition();
		const Math::point direction = DirectX::XMVector2Normalize((Velocity - current).vec);
		const Math::point target = current + direction * GameTime::Instance()->deltaTime;
		Transform.setposition(target);
	}

	void TransformComponent::Shutdown()
	{
	}

	bool TransformComponent::HandleMessage(const Telegram& msg)
	{
		return true;
	}

	void TransformComponent::Translate(Math::float4 position)
	{
		Velocity += DirectX::XMVector4Normalize((position - Velocity).vec);
	}

	void TransformComponent::SetPosition(Math::float4 position)
	{
		Transform.setposition(position);
	}

	void TransformComponent::SetRotation(float x, float y, float z)
	{
		const Math::quaternion quat = Math::quaternion::rotationyawpitchroll(x, y, z);
		Transform.setrotate(quat);
	}

	void TransformComponent::SetScale(Math::float4 scale)
	{
		// Transform.setrotate();
	}
}

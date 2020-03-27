#include "stdneb.h"
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
		Rotation = Transform.getrotate();
		
		Owner->RegisterVariable("Velocity", Velocity);
		Owner->RegisterVariable("Rotation", Rotation);
	}

	void TransformComponent::Update()
	{
		// const Math::point current = Transform.getposition();
		// const Math::point direction = DirectX::XMVector2Normalize((Velocity - current).vec);
		// const Math::point target = current + direction * GameTime::Instance()->deltaTime;
		Transform.setposition(Velocity);
		Transform.setrotate(Rotation);
	}

	void TransformComponent::Shutdown()
	{
		
	}

	bool TransformComponent::HandleMessage(const Telegram& msg)
	{
		return true;
	}

	void TransformComponent::Translate(const Math::float4 direction)
	{
		Velocity += direction;
	}

	void TransformComponent::Rotate(float x, float y, float z, float angle)
	{
		const Math::quaternion currentRotation = Transform.getrotate();
		Rotation = Math::quaternion::multiply(currentRotation, Math::quaternion::rotationaxis(Math::float4(x, y, z, 0), angle));
	}

	void TransformComponent::SetPosition(const Math::float4 position)
	{
		Velocity = position; 
	}

	void TransformComponent::SetRotation(const float x, const float y, const float z)
	{
		const Math::quaternion quat = Math::quaternion::rotationyawpitchroll(x, y, z);
		Transform.setrotate(quat);
	}

	void TransformComponent::SetScale(const Math::float4 scale)
	{
		 Transform.setscale(scale);
	}
}

#pragma once
#include "Component.h"
#include "core/refcounted.h"

namespace GameEngine
{
	class TransformComponent final : public Component, public Core::RefCounted
	{
		__DeclareClass(GameEngine::TransformComponent)

	public:

		enum class Orientation { X, Y, Z };
		
		Math::float4 Velocity;
		Math::transform44 Transform;
		Math::quaternion  Rotation;

		void Init() override;
		void Update() override;
		void Shutdown() override;
		bool HandleMessage(const Telegram& msg) override;

		void Translate(Math::float4 direction);
		void Rotate(float x, float y, float z, float angle);

		void SetPosition(Math::float4 position);
		void SetRotation(float x, float y, float z);
		void SetScale(Math::float4 scale);

		
	};
}

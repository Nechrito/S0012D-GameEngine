#pragma once
#include "stdneb.h"
#include "Component.h"
#include "core/refcounted.h"

namespace GameEngine
{
	class TransformComponent final : public Component, public Core::RefCounted
	{
		__DeclareClass(GameEngine::TransformComponent)

	public:
		Math::float4 Velocity;
		Math::transform44 Transform;

		void Init() override;
		void Update() override;
		void Shutdown() override;
		bool HandleMessage(const Telegram& msg) override;

		void Translate(Math::float4 position);
		
		void SetPosition(Math::float4 position);
		void SetRotation(float x, float y, float z);
		void SetScale(Math::float4 scale);

		
	};
}

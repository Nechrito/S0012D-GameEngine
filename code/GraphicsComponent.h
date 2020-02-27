#pragma once
#include "stdneb.h"
#include "Component.h"
#include "core/rttimacros.h"
#include "core/refcounted.h"
#include "graphics/graphicsentity.h"
#include "resources/resource.h"

namespace GameEngine
{
	class GraphicsComponent final : public Component, public Core::RefCounted
	{
		__DeclareClass(GraphicsComponent)

	public:
		Graphics::GraphicsEntityId ID;

		GraphicsComponent() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;
		bool HandleMessage(const Telegram& msg) override;
		
		void Load(const Resources::ResourceName& resource, const Util::StringAtom& tag);

		~GraphicsComponent() = default;
	};
}

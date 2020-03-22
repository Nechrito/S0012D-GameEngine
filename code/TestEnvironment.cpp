#include "TestEnvironment.h"
#include "TransformComponent.h"
#include "EntityManager.h"
#include "BaseEntity.h"
#include "GameTime.h"

namespace GameEngine
{
	__ImplementClass(TestEnvironment, 'GETE', Core::RefCounted)
	__ImplementSingleton(TestEnvironment)

	TestEnvironment::TestEnvironment()
	{
		__ConstructSingleton
	}

	TestEnvironment::~TestEnvironment()
	{
		__DestructSingleton
	}

	void TestEnvironment::Initialize()
	{
		GameTime::Create();
		MessageDispatcher::Create();
		EntityManager::Create();

		EntityManager::Instance()->RegisterEntity("ground", "mdl:environment/Groundplane.n3", "Examples");
		EntityManager::Instance()->RegisterEntity("catapult", "mdl:Units/Unit_Catapult.n3", "Examples");
	}

	void TestEnvironment::Update()
	{
		GameTime::Instance()->Update();
		EntityManager::Instance()->Update();

		BaseEntity* entity1 = EntityManager::Instance()->GetEntity("catapult");
		if (entity1)
		{
			TransformComponent* entityTransform = dynamic_cast<TransformComponent*>(entity1->GetComponent("Transform"));
			if (entityTransform)
			{
				entityTransform->Translate(Math::point(0, 0, 0.005));
				entityTransform->Rotate(0, 1, 0, 0.01);
			}
		}
	}

	void TestEnvironment::Draw()
	{
		
	}

	void TestEnvironment::Unload()
	{
		EntityManager::Instance()->Shutdown();
	}
}

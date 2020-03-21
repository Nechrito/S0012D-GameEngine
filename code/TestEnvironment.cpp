#include "TestEnvironment.h"
#include "TransformComponent.h"
#include "EntityManager.h"
#include "BaseEntity.h"

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
		EntityManager::Create();
		EntityManager::Instance()->RegisterEntity("ground", "mdl:environment/Groundplane.n3", "Examples");
		EntityManager::Instance()->RegisterEntity("catapult", "mdl:Units/Unit_Catapult.n3", "Examples");
	}

	void TestEnvironment::Update()
	{
		EntityManager::Instance()->Update();

		BaseEntity* entity1 = EntityManager::Instance()->GetEntity("catapult");
		if (entity1)
		{
			TransformComponent* entityTransform = dynamic_cast<TransformComponent*>(entity1->GetComponent("Transform"));
			if (entityTransform)
				entityTransform->Translate(Math::point(0, 0, -10));
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

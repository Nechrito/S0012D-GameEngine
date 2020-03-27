#include "stdneb.h"
#include "TestEnvironment.h"
#include "GameTime.h"
#include "MessageDispatcher.h"
#include "EntityManager.h"
#include "TransformComponent.h"
#include "BaseEntity.h"
#include "scripting/python/pythonserver.h"
#include "PyScripting.h"


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

		// Instantiate a few entities in python script, then move it in C++ down below @Update()
		EntityManager::Instance()->RegisterEntity("Ground", "mdl:environment/Groundplane.n3", "Examples");
		//EntityManager::Instance()->RegisterEntity("Catapult", "mdl:Units/Unit_Catapult.n3", "Examples");
		//EntityManager::Instance()->RegisterEntity("Footman", "mdl:Units/Unit_Footman.n3", "Examples", Math::point(5, 0, 0));

		//EntityManager::Instance()->Init();

		// Scripting
		Scripting::PythonServer* pyServer = Scripting::PythonServer::Create();
		pyServer->Open();

		const Util::String uri("root:code/PythonScript.py");
		pyServer->EvalFile(uri);
	}

	void TestEnvironment::Update()
	{
		GameTime::Instance()->Update();
		EntityManager::Instance()->Update();

		BaseEntity* entity1 = EntityManager::Instance()->GetEntity("Catapult");
		if (entity1)
		{
			TransformComponent* entityTransform = dynamic_cast<TransformComponent*>(entity1->GetComponent("Transform"));
			if (entityTransform)
			{
				const double dt = GameTime::Instance()->deltaTime;
				t += dt;

				const float moveSpeed = sinf(t * 3.1415);
				
				entityTransform->Translate(Math::point(0, -moveSpeed * dt, 0));
				entityTransform->Rotate(0, 1, 0, 0.01);
			}
		}
	}

	void TestEnvironment::Unload()
	{
		EntityManager::Instance()->Shutdown();
	}
}

#pragma once
#include "core/refcounted.h"
#include "core/singleton.h"

namespace GameEngine
{
	class TestEnvironment : public Core::RefCounted
	{
		__DeclareClass(TestEnvironment)
		__DeclareSingleton(TestEnvironment)
		
	public:

		TestEnvironment();
		~TestEnvironment();

		void Initialize();
		void Update();
		void Unload();


	private:

		float t = 0;
		float s = 0;
		
	};
}


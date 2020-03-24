# 3.7.2 required for using FindVulkan
cmake_minimum_required(VERSION 3.7.2)

get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include(${FIPS_ROOT_DIR}/cmake/fips.cmake)

fips_setup(PROJECT GameEngine-project)
fips_include_directories(.)

nebula_begin_app(GameEngine windowed)
    
    fips_dir(code)
        fips_files(
           main.cc
           Engine.cc
		   Engine.h
		   BaseEntity.cpp
		   BaseEntity.h
		   Component.h
		   EntityManager.cpp
		   EntityManager.h
		   GameTime.cpp
		   GameTime.h
		   GraphicsComponent.cpp
		   GraphicsComponent.h
		   MessageDispatcher.cpp
		   MessageDispatcher.h
		   Telegram.cpp
		   Telegram.h
		   TestEnvironment.cpp
		   TestEnvironment.h
		   Scripting.cpp
		   Scripting.h
		   TransformComponent.cpp
		   TransformComponent.h
        )

    fips_deps(foundation render resource dynui scripting)
nebula_end_app()

fips_finish()

#include "stdneb.h"
#include "foundation.h"
#include "core/coreserver.h"
#include "core/sysfunc.h"
#include "Engine.h"
#include "system/appentry.h"

ImplementNebulaApplication()

void NebulaMain(const Util::CommandLineArgs& args)
{
    GameEngine::Engine app;

	app.SetCmdLineArgs(args);
	
    if (app.Open())
    {
        app.Run();
        app.Close();
    }

    Core::SysFunc::Exit(0);
}

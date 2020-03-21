#include "stdneb.h"
#include "core/refcounted.h"
#include "timing/timer.h"
#include "io/console.h"
#include "visibility/visibilitycontext.h"
#include "models/streammodelpool.h"
#include "models/modelcontext.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "Engine.h"
#include "math/vector.h"
#include "math/point.h"
#include "dynui/imguicontext.h"
#include "lighting/lightcontext.h"
#include "characters/charactercontext.h"
#include "imgui.h"
#include "dynui/im3d/im3dcontext.h"
#include "dynui/im3d/im3d.h"
#include "graphics/environmentcontext.h"
#include "clustering/clustercontext.h"
#include "io/debug/iopagehandler.h"
#include "io/logfileconsolehandler.h"
#include "io/debug/consolepagehandler.h"
#include "core/debug/corepagehandler.h"
#include "threading/debug/threadpagehandler.h"
#include "memory/debug/memorypagehandler.h"
#include "io/debug/iopagehandler.h"
#include "io/fswrapper.h"
#include "system/nebulasettings.h"
#include "profiling/profiling.h"
#include "EntityManager.h"
#include "BaseEntity.h"
#include "GameTime.h"
#include "TransformComponent.h"
#include "TestEnvironment.h"


#ifdef __WIN32__
#include <shellapi.h>
#elif __LINUX__

#endif

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;
using namespace IO;
using namespace Http;
using namespace Debug;

namespace GameEngine
{

    Engine::Engine() : defaultTcpPort(2100)
    {
        this->SetAppTitle("Viewer App");
        this->SetCompanyName("Nebula");
    }

    bool Engine::Open()
    {
        if (Application::Open())
        {
            // create Nebula runtime
            this->coreServer = Core::CoreServer::Create();
            this->coreServer->SetCompanyName(Instance()->GetCompanyName());
            this->coreServer->SetAppName(Util::StringAtom("Nebula"));

            this->inputServer = Input::InputServer::Create();
            this->ioServer = IoServer::Create();

            this->ioInterface = IoInterface::Create();
            this->ioInterface->Open();

            this->resMgr = Resources::ResourceManager::Create();
            this->resMgr->Open();

            Util::String root = FSWrapper::GetHomeDirectory();

#if PUBLIC_BUILD
            if (System::NebulaSettings::Exists(Application::Instance()->GetCompanyName(), Application::Instance()->GetAppTitle(), "path"))
            {
                root = System::NebulaSettings::ReadString(Application::Instance()->GetCompanyName(), Application::Instance()->GetAppTitle(), "path");
            }
#else 
            if (System::NebulaSettings::Exists("gscept", "ToolkitShared", "workdir"))
            {
                root = System::NebulaSettings::ReadString("gscept", "ToolkitShared", "workdir");
            }
            if (System::NebulaSettings::Exists("gscept", "ToolkitShared", "path"))
            {
                this->coreServer->SetToolDirectory(System::NebulaSettings::ReadString("gscept", "ToolkitShared", "path"));
            }
#endif

            //n_assert2(System::NebulaSettings::ReadString("gscept", "ToolkitShared", "workdir"), "No working directory defined!");

            this->coreServer->SetRootDirectory(root);
            this->coreServer->Open();


#if __WIN32__
            const Ptr<LogFileConsoleHandler> logFileHandler = LogFileConsoleHandler::Create();
            Console::Instance()->AttachHandler(logFileHandler.upcast<ConsoleHandler>());
#endif

#if __NEBULA_HTTP_FILESYSTEM__
            // setup http subsystem
            this->httpClientRegistry = HttpClientRegistry::Create();
            this->httpClientRegistry->Setup();
#endif

#if __NEBULA_HTTP__
            // setup http subsystem
            this->httpInterface = HttpInterface::Create();
            this->httpInterface->SetTcpPort(this->defaultTcpPort);
            this->httpInterface->Open();
            this->httpServerProxy = HttpServerProxy::Create();
            this->httpServerProxy->Open();
            this->httpServerProxy->AttachRequestHandler(CorePageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(ThreadPageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(MemoryPageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(ConsolePageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(IoPageHandler::Create());

            // setup debug subsystem
            this->debugInterface = DebugInterface::Create();
            this->debugInterface->Open();
#endif

            this->gfxServer = GraphicsServer::Create();
            this->inputServer->Open();
            this->gfxServer->Open();

            const SizeT width = this->GetCmdLineArgs().GetInt("-w", 1024);
            const SizeT height = this->GetCmdLineArgs().GetInt("-h", 768);

            const CoreGraphics::WindowCreateInfo wndInfo =
            {
                CoreGraphics::DisplayMode{ 100, 100, width, height },
                this->GetAppTitle(), "", CoreGraphics::AntiAliasQuality::None, true, true, false
            };
            this->wnd = CreateWindow(wndInfo);
            this->cam = CreateEntity();

#if NEBULA_ENABLE_PROFILING
            Profiling::ProfilingRegisterThread();
#endif

            // Create contexts, this could and should be bundled together
            CameraContext::Create();
            ModelContext::Create();
            ObserverContext::Create();
            ObservableContext::Create();

            GameTime::Create();
            MessageDispatcher::Create();
            TestEnvironment::Create();
        	
            TestEnvironment::Instance()->Initialize();

            // Camera entity
            Graphics::RegisterEntity<CameraContext, ObserverContext>(this->cam);

        	// Projection
            CameraContext::SetupProjectionFov(this->cam, width / (float)height, Math::n_deg2rad(fov), znear, zfar);

        	Clustering::ClusterContext::Create(znear, zfar, this->wnd);
        	Lighting::LightContext::Create();
            Characters::CharacterContext::Create();
            Im3d::Im3dContext::Create();
            Dynui::ImguiContext::Create();

            this->view = gfxServer->CreateView("mainview", "frame:vkdefault.json"_uri);
            this->stage = gfxServer->CreateStage("stage1", true);

            this->globalLight = CreateEntity();
            Lighting::LightContext::RegisterEntity(this->globalLight);
            Lighting::LightContext::SetupGlobalLight(this->globalLight, Math::float4(1, 1, 1, 0), 1.0f, Math::float4(0, 0, 0, 0), Math::float4(0, 0, 0, 0), 0.0f, -Math::vector(1, 1, 1), true);

        	
            this->ResetCamera();
            CameraContext::SetTransform(this->cam, this->mayaCameraUtil.GetCameraTransform());

            this->view->SetCamera(this->cam);
            this->view->SetStage(this->stage);

            // Register visibility system
            ObserverContext::CreateBruteforceSystem({});

            ObserverContext::Setup(this->cam, VisibilityEntityType::Camera);

            // Create environment context for the atmosphere effects
            EnvironmentContext::Create(this->globalLight);

            this->UpdateCamera();

            return true;
        }
        return false;
    }

    void Engine::Close()
    {

        TestEnvironment::Instance()->Unload();

    	
        Application::Close();
        DestroyWindow(this->wnd);
        this->gfxServer->DiscardStage(this->stage);
        this->gfxServer->DiscardView(this->view);

        ObserverContext::Discard();
        Lighting::LightContext::Discard();

        this->gfxServer->Close();
        this->inputServer->Close();
        this->resMgr->Close();

        this->ioInterface->Close();
        this->ioInterface = nullptr;

        this->ioServer = nullptr;

#if __NEBULA_HTTP__
        this->debugInterface->Close();
        this->debugInterface = nullptr;

        this->httpServerProxy->Close();
        this->httpServerProxy = nullptr;
        this->httpInterface->Close();
        this->httpInterface = nullptr;
#endif

#if __NEBULA_HTTP_FILESYSTEM__
        this->httpClientRegistry->Discard();
        this->httpClientRegistry = nullptr;
#endif

        this->coreServer->Close();
        this->coreServer = nullptr;

    }

    void Engine::Run()
    {
        bool run = true;

        const Ptr<Input::Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
        const Ptr<Input::Mouse>& mouse = inputServer->GetDefaultMouse();

        // Create a point light entity
        const GraphicsEntityId pointLight = CreateEntity();
    	
        // You can also register to contexts directly
        Lighting::LightContext::RegisterEntity(pointLight);
        Lighting::LightContext::SetupPointLight(pointLight, Math::float4(4.5, 0.5, 0.2, 1), 10.0f, Math::matrix44::translation(1, 2, 1), 100.0f, true);

        while (run && !inputServer->IsQuitRequested())
        {
#if NEBULA_ENABLE_PROFILING
            Profiling::ProfilingNewFrame();
#endif

#if __NEBULA_HTTP__
            this->httpServerProxy->HandlePendingRequests();
#endif

            this->coreServer->Trigger();

            this->inputServer->BeginFrame();
            this->inputServer->OnFrame();

            this->resMgr->Update(this->frameIndex);

            this->gfxServer->BeginFrame();

            // put game code which doesn't need visibility data or animation here
            TestEnvironment::Instance()->Update();
            
        	
            this->gfxServer->BeforeViews();

            this->RenderUI();

            if (this->renderDebug)
            {
                this->gfxServer->RenderDebug(0);
            }

            // put game code which need visibility data here
            this->gfxServer->RenderViews();

            // put game code which needs rendering to be done (animation etc) here
            this->gfxServer->EndViews();

            // do stuff after rendering is done
            this->gfxServer->EndFrame();

            // force wait immediately
            WindowPresent(wnd, frameIndex);
            if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::Escape)) run = false;

            if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::LeftMenu))
                this->UpdateCamera();

            if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::F8))
                Resources::ResourceManager::Instance()->ReloadResource("shd:imgui.fxb");

            if (this->inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::F1))
            {
                // Open browser with debug page.
                Util::String url = "http://localhost:";
                url.AppendInt(this->defaultTcpPort);
#ifdef __WIN32__
                ShellExecute(0, 0, url.AsCharPtr(), 0, 0, SW_SHOW);
#elif __LINUX__
                Util::String shellCommand = "open ";
                shellCommand.Append(url);
                system(shellCommand.AsCharPtr());
#else
                n_printf("Cannot open browser. URL is %s\n", url.AsCharPtr());
#endif
            }

            frameIndex++;
            this->inputServer->EndFrame();
        }
    }

    void Engine::RenderUI()
    {
        this->averageFrameTime += (float)this->gfxServer->GetFrameTime();
        if (this->gfxServer->GetFrameIndex() % 35 == 0)
        {
            this->prevAverageFrameTime = this->averageFrameTime / 35.0f;
            this->averageFrameTime = 0.0f;
        }
        ImGui::Begin("Debug", nullptr, 0);
        ImGui::Text("ms - %.2f\nFPS - %.2f", this->prevAverageFrameTime * 1000, 1 / this->prevAverageFrameTime);
        ImGui::SetWindowSize(ImVec2(240, 400));
        if (ImGui::CollapsingHeader("Camera mode", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::RadioButton("Maya", &this->cameraMode, 0))this->ToMaya();
            ImGui::SameLine();
            if (ImGui::RadioButton("Free", &this->cameraMode, 1))this->ToFree();
            ImGui::SameLine();
            if (ImGui::Button("Reset")) this->ResetCamera();
        }
        ImGui::Checkbox("Debug Rendering", &this->renderDebug);

        ImGui::End();
    }

    void Engine::UpdateCamera()
    {
        const Ptr<Input::Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
        const Ptr<Input::Mouse>& mouse = inputServer->GetDefaultMouse();

        this->mayaCameraUtil.SetOrbitButton(mouse->ButtonPressed(Input::MouseButton::LeftButton));
        this->mayaCameraUtil.SetPanButton(mouse->ButtonPressed(Input::MouseButton::MiddleButton));
        this->mayaCameraUtil.SetZoomButton(mouse->ButtonPressed(Input::MouseButton::RightButton));
        this->mayaCameraUtil.SetZoomInButton(mouse->WheelForward());
        this->mayaCameraUtil.SetZoomOutButton(mouse->WheelBackward());
        this->mayaCameraUtil.SetMouseMovement(mouse->GetMovement());

        // process keyboard input
        Math::float4 pos(0.0f);
        if (keyboard->KeyDown(Input::Key::Space))
        {
            this->mayaCameraUtil.Reset();
        }
        if (keyboard->KeyPressed(Input::Key::Left))
        {
            panning.x() -= 0.1f;
            pos.x() -= 0.1f;
        }
        if (keyboard->KeyPressed(Input::Key::Right))
        {
            panning.x() += 0.1f;
            pos.x() += 0.1f;
        }
        if (keyboard->KeyPressed(Input::Key::Up))
        {
            panning.y() -= 0.1f;
            if (keyboard->KeyPressed(Input::Key::LeftShift))
            {
                pos.y() -= 0.1f;
            }
            else
            {
                pos.z() -= 0.1f;
            }
        }
        if (keyboard->KeyPressed(Input::Key::Down))
        {
            panning.y() += 0.1f;
            if (keyboard->KeyPressed(Input::Key::LeftShift))
            {
                pos.y() += 0.1f;
            }
            else
            {
                pos.z() += 0.1f;
            }
        }

        this->mayaCameraUtil.SetPanning(panning);
        this->mayaCameraUtil.SetOrbiting(orbiting);
        this->mayaCameraUtil.SetZoomIn(zoomIn);
        this->mayaCameraUtil.SetZoomOut(zoomOut);
        this->mayaCameraUtil.Update();

        this->freeCamUtil.SetForwardsKey(keyboard->KeyPressed(Input::Key::W));
        this->freeCamUtil.SetBackwardsKey(keyboard->KeyPressed(Input::Key::S));
        this->freeCamUtil.SetRightStrafeKey(keyboard->KeyPressed(Input::Key::D));
        this->freeCamUtil.SetLeftStrafeKey(keyboard->KeyPressed(Input::Key::A));
        this->freeCamUtil.SetUpKey(keyboard->KeyPressed(Input::Key::Q));
        this->freeCamUtil.SetDownKey(keyboard->KeyPressed(Input::Key::E));

        this->freeCamUtil.SetMouseMovement(mouse->GetMovement());
        this->freeCamUtil.SetAccelerateButton(keyboard->KeyPressed(Input::Key::LeftShift));

        this->freeCamUtil.SetRotateButton(mouse->ButtonPressed(Input::MouseButton::LeftButton));
        this->freeCamUtil.Update();

    	switch (this->cameraMode)
        {
        case 0:
            CameraContext::SetTransform(this->cam, Math::matrix44::inverse(this->mayaCameraUtil.GetCameraTransform()));
            break;
        case 1:
            CameraContext::SetTransform(this->cam, Math::matrix44::inverse(this->freeCamUtil.GetTransform()));
            break;
        default:
            break;
        }
    }

    void Engine::ResetCamera()
    {
        this->freeCamUtil.Setup(this->defaultViewPoint, Math::float4::normalize(this->defaultViewPoint));
        this->freeCamUtil.Update();
        this->mayaCameraUtil.Setup(Math::point(0.0f, 0.0f, 0.0f), this->defaultViewPoint, Math::vector(0.0f, 1.0f, 0.0f));
    }

    void Engine::ToMaya()
    {
        this->mayaCameraUtil.Setup(this->mayaCameraUtil.GetCenterOfInterest(), this->freeCamUtil.GetTransform().get_position(), Math::vector(0, 1, 0));
    }

    void Engine::ToFree()
    {
	    const Math::float4 pos = this->mayaCameraUtil.GetCameraTransform().get_position();
        this->freeCamUtil.Setup(pos, Math::float4::normalize(pos - this->mayaCameraUtil.GetCenterOfInterest()));
    }

}

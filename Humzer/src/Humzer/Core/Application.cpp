#include "humpch.h"

#include "Application.h"

#include "GLFW\glfw3.h"

#include <memory>

#include <filesystem>
#include "..\Renderer\RenderCommand.h"
#include "..\Renderer\Renderer.h"
#include "..\Events\Dispatcher.h"
#include "..\ImGui\ImGuiRuntime.h"


namespace Humzer {

    Application* Application::s_Instance = nullptr;

    Application::Application() {

        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create(1280, 720, "Humzer Game Engine"));

        GUIRuntime = new ImGuiRuntime();

        Dispatcher::Initialize();

	}

    Application::~Application(){
        GUIRuntime->OnShutdown();
        delete GUIRuntime;
	}

    void Application::Run(){

        RenderCommand::Init();
        
        Renderer2D::Init();
        Renderer3D::Init();

        GUIRuntime->OnStart();
        ClientOnStart();

        while (m_Running) {
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

            GUIRuntime->Begin();
            ClientUpdate(timestep);
            GUIRuntime->End();

            m_Window->OnUpdate();
        }

        Renderer2D::Shutdown();
        Renderer3D::Shutdown();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(750));

        Dispatcher::Stop();
        Dispatcher::Terminate();
    }

    void Application::Quit()
    {
        m_Running = false;
    }

}

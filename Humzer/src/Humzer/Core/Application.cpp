#include "humpch.h"

#include "Application.h"

#include "GLFW\glfw3.h"

// TESTING ONLY
#include <memory>
#include <Humzer\Renderer\VertexArray.h>

#include <filesystem>
#include "..\Renderer\PrimitiveData.h"
#include "..\Platform\OpenGL\OpenGLShader.h"
#include "..\Renderer\RenderCommand.h"
#include "..\Renderer\Renderer.h"
#include "glm\ext\matrix_transform.hpp"


namespace Humzer {

    Application* Application::s_Instance = nullptr;

    Application::Application() {

        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create(1280, 720, "Humzer Game Engine"));

        basicCam = new PerspectiveCamera(45.0f, 1.778f, 0.1f, 1000.0f, glm::vec3(0.0f, 4.0f, 20.0f), true);

	}
    Application::~Application(){}

    void Application::Run(){

        RenderCommand::EnableDepthTesting();

        Renderer3D::Init();

        while (m_Running) {

            RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
            RenderCommand::Clear();

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

            basicCam->OnUpdate(timestep);

            Renderer3D::BeginScene(*basicCam);

            ClientUpdate(timestep);

            Renderer3D::EndScene();

            m_Window->OnUpdate();
        }

        Renderer3D::Shutdown();
    }

}

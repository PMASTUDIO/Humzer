#include "humpch.h"

#include "Application.h"

#include <gl\GL.h>

#include "GLFW\glfw3.h"

namespace Humzer {

    Application* Application::s_Instance = nullptr;

    Application::Application(){

        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create(1280, 720, "Humzer Game Engine"));

	}
    Application::~Application(){}

    void Application::Run(){
        while (m_Running) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

            ClientUpdate(timestep);

            m_Window->OnUpdate();
        }
    }

}

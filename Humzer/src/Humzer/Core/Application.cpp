#include "humpch.h"

#include "Application.h"

namespace Humzer {

    Application::Application(){
        m_Window = std::unique_ptr<Window>(Window::Create(1280, 720, "Humzer Game Engine"));
	}
    Application::~Application(){}

    void Application::Run(){
        while (m_Running) {
            m_Window->OnUpdate();
        }
    }
    
}

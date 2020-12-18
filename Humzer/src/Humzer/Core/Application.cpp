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

        basicCam = new PerspectiveCamera(45.0f, 1.778f, 0.1f, 1000.0f, glm::vec3(0.0f, 4.0f, 20.0f));

	}
    Application::~Application(){}

    void Application::Run(){

        //// LOADING ASSETS
        CheckerboardTexture = Texture2D::Create("Resources/textures/Checkerboard.png");
        
        //// BIND TEXTURE UNIFORM
        //BasicShader->Bind();
        //BasicShader->SetInt("u_Texture", 0); // BIND SLOT

        //glm::mat4 modelPos = glm::mat4(1.0);

        RenderCommand::EnableDepthTesting();

        Renderer3D::Init();

        while (m_Running) {

            RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
            RenderCommand::Clear();

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

            basicCam->OnUpdate(timestep);

            ClientUpdate(timestep);

            // TESTING
            Renderer3D::BeginScene(*basicCam);
            
            //CheckerboardTexture->Bind();
            Renderer3D::DrawPlane({ 0.0, 0.0, 0.0 }, { 1.0, 1.0 }, { 1.0, 0.0, 0.0, 1.0 });
            Renderer3D::DrawCube({ 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 });

           /* modelPos = glm::rotate(modelPos, 2 * timestep, glm::vec3(1.0, 0.4, 0.2));*/
            Renderer::EndScene();
            
            m_Window->OnUpdate();
        }

        Renderer3D::Shutdown();
    }

}

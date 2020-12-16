#include "humpch.h"

#include "Application.h"

#include "GLFW\glfw3.h"

// TESTING ONLY
#include <Humzer\Renderer\VertexArray.h>

namespace Humzer {

    Application* Application::s_Instance = nullptr;

    Application::Application(){

        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create(1280, 720, "Humzer Game Engine"));
	}
    Application::~Application(){}

    void Application::Run(){
        // TESTING ONLY (VERTEX STUFF)

        VAO = VertexArray::Create();

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f,     0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f,     0.2f, 0.3f, 0.2f, 1.0f,
             0.0f,  0.5f, 0.0f,     0.6f, 0.6f, 0.2f, 1.0f,
        };
        VBO = VertexBuffer::Create(vertices, sizeof(vertices));

        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position"},
            { ShaderDataType::Float4, "a_Color"},
        };
        VBO->SetLayout(layout);
        VAO->AddVertexBuffer(VBO);

        uint32_t indices[3] = { 0, 1, 2 };
        EBO = IndexBuffer::Create(indices, sizeof(indices));
        VAO->SetIndexBuffer(EBO);

        while (m_Running) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

            ClientUpdate(timestep);

            // TESTING
            VAO->Bind();
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
            VAO->Unbind();

            m_Window->OnUpdate();
        }
    }

}

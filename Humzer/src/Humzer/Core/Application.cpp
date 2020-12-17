#include "humpch.h"

#include "Application.h"

#include "GLFW\glfw3.h"

// TESTING ONLY
#include <memory>
#include <Humzer\Renderer\VertexArray.h>

#include <filesystem>
#include "..\Renderer\PrimitiveData.h"
#include "..\Platform\OpenGL\OpenGLShader.h"


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

        std::vector<float> vertices = {};
        std::vector<uint32_t> indices = {};

        Primitive::getQuadData(vertices, indices);
        VBO = VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float));

        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position"},
            { ShaderDataType::Float3, "a_Normals"},
            { ShaderDataType::Float4, "a_Color"},
            { ShaderDataType::Float2, "a_TexCoord"},
        };

        VBO->SetLayout(layout);
        VAO->AddVertexBuffer(VBO);

        EBO = IndexBuffer::Create(&indices[0], indices.size() * sizeof(uint32_t));
        VAO->SetIndexBuffer(EBO);

        // LOADING ASSETS
		BasicShader = Shader::Create("Resources/shaders/textured_shader.vs", "Resources/shaders/textured_shader.fs");
        CheckerboardTexture = Texture2D::Create("Resources/textures/Checkerboard.png");

        // BIND TEXTURE UNIFORM
        std::dynamic_pointer_cast<OpenGLShader>(BasicShader)->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(BasicShader)->UploadUniformInt("u_Texture", 0); // BIND SLOT

        while (m_Running) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

            ClientUpdate(timestep);

            // TESTING
            CheckerboardTexture->Bind();

			BasicShader->Bind();
            VAO->Bind();
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            VAO->Unbind();
			BasicShader->Unbind();

            m_Window->OnUpdate();
        }
    }

}

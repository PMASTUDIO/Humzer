#include "humpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "../Platform/OpenGL/OpenGLShader.h"

namespace Humzer {

	Humzer::Ref<Humzer::Shader> Shader::Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vertexPath, fragmentPath);
			break;
		default:
		case RendererAPI::API::None:
			HUM_CORE_FATAL("No Renderer API Selected!");
			return nullptr;
		}
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();

		// #ASSERT
		if (m_Shaders.find(name) != m_Shaders.end()) {
			HUM_CORE_WARN("Shader with name {0} already exists!", name);
			return;
		}

		m_Shaders[name] = shader;
	}

	Humzer::Ref<Humzer::Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
	{
		auto shader = Shader::Create(name, vertexPath, fragmentPath);
		Add(shader);
		return shader;
	}

	Humzer::Ref<Humzer::Shader> ShaderLibrary::Get(const std::string& name)
	{
		// #ASSERT
		if (m_Shaders.find(name) != m_Shaders.end()) {
			HUM_CORE_WARN("Shader with name {0} already exists!", name);
			return nullptr;
		}

		return m_Shaders[name];
	}

}

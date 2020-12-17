#include "humpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "../Platform/OpenGL/OpenGLShader.h"

namespace Humzer {

	Humzer::Ref<Humzer::Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLShader>(vertexPath, fragmentPath);
			break;
		default:
		case RendererAPI::None:
			HUM_CORE_FATAL("No Renderer API Selected!");
			return nullptr;
		}
	}

}

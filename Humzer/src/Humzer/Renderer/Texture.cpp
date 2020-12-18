#include "humpch.h"
#include "Texture.h"

#include "Renderer.h"
#include <Humzer\Platform\OpenGL\OpenGLTexture.h>

namespace Humzer {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
			break;
		default:
		case RendererAPI::API::None:
			HUM_CORE_FATAL("No Renderer API Selected!");
			return nullptr;
		}
	}

}

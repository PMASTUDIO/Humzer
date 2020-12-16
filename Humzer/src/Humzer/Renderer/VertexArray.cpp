#include "humpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "../Platform/OpenGL/OpenGLVertexArray.h"


namespace Humzer {

	Humzer::Ref<Humzer::VertexArray> Humzer::VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return CreateRef<OpenGLVertexArray>();
			break;
		default:
		case RendererAPI::None:
			HUM_CORE_FATAL("No Renderer API Selected!");
			return nullptr;
		}
	}

}

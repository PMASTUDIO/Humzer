#include "humpch.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "RendererAPI.h"
#include "../Platform/OpenGL/OpenGLFramebuffer.h"

namespace Humzer {

	Humzer::Ref<Humzer::Framebuffer> Framebuffer::Create(const FramebufferSpecs& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
			break;
		default:
		case RendererAPI::API::None:
			HUM_CORE_FATAL("No Renderer API Selected!");
			return nullptr;
		}
	}

}

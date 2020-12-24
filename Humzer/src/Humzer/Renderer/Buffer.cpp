#include "humpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "../Platform/OpenGL/OpenGLBuffer.h"

Humzer::Ref<Humzer::VertexBuffer> Humzer::VertexBuffer::Create(void* data, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLVertexBuffer>(data, size);
		break;


	default:
	case RendererAPI::API::None:
		HUM_CORE_FATAL("No Renderer API Selected!");
		return nullptr;
	}
}

Humzer::Ref<Humzer::IndexBuffer> Humzer::IndexBuffer::Create(void* indices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLIndexBuffer>(indices, size);
		break;


	default:
	case RendererAPI::API::None:
		HUM_CORE_FATAL("No Renderer API Selected!");
		return nullptr;
	}
}

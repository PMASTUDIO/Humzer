#include "humpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "../Platform/OpenGL/OpenGLBuffer.h"

Humzer::Ref<Humzer::VertexBuffer> Humzer::VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLVertexBuffer>(vertices, size);
		break;


	default:
	case RendererAPI::API::None:
		HUM_CORE_FATAL("No Renderer API Selected!");
		return nullptr;
	}
}

Humzer::Ref<Humzer::IndexBuffer> Humzer::IndexBuffer::Create(uint32_t* indices, uint32_t size)
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

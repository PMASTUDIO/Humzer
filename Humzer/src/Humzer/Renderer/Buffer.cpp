#include "humpch.h"
#include "Buffer.h"

#include "Renderer.h"

Humzer::VertexBuffer* Humzer::VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None:
		HUM_CORE_FATAL("No Renderer API Selected!");
		return nullptr;
	default:
		break;
	}
}

Humzer::IndexBuffer* Humzer::IndexBuffer::Create(float* indices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None:
		HUM_CORE_FATAL("No Renderer API Selected!");
		return nullptr;
	default:
		break;
	}
}
 
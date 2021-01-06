#pragma once

#include "Humzer/Renderer/Buffer.h"

namespace Humzer {

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(void* data, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		const BufferLayout& GetLayout() const override { return m_Layout; }

		void SetData(const void* data, uint32_t size) override;

	private:
		unsigned int m_ID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(void* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		virtual uint32_t GetCount() const { return m_Size / sizeof(uint32_t); }
		virtual uint32_t GetSize() const { return m_Size; }

	private:
		unsigned int m_ID;
		uint32_t m_Size;
	};

}

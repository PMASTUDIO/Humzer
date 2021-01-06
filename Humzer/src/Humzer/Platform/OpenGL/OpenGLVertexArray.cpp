#include "humpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

Humzer::OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &m_ID);
}

Humzer::OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void Humzer::OpenGLVertexArray::Bind() const
{
	glBindVertexArray(m_ID);
}

void Humzer::OpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void Humzer::OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
	glBindVertexArray(m_ID);
	vertexBuffer->Bind();

	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout)
	{
		glEnableVertexAttribArray(m_VertexBufferIndex);
		glVertexAttribPointer(m_VertexBufferIndex,
			element.GetComponentCount(),
			ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			(const void*)element.Offset);
		m_VertexBufferIndex++;
	}

	m_VertexBuffers.push_back(vertexBuffer);
}

void Humzer::OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(m_ID);
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer;
}

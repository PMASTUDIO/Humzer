#pragma once
#include "../../Renderer/Framebuffer.h"

namespace Humzer {

	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpecs& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		void Bind() override;
		void Unbind() override;

		const FramebufferSpecs& GetSpecs() const override { return m_Specs; };

		uint32_t GetColorAttachment() const override { return m_ColorAttachment; }

		void Resize(uint32_t width, uint32_t height) override;

	private:
		unsigned int m_ID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		uint32_t m_IDBufferAttachment = 0;
		FramebufferSpecs m_Specs;
	};

}

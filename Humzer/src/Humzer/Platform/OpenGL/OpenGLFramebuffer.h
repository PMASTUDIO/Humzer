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

	private:
		unsigned int m_ID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferSpecs m_Specs;
	};

}

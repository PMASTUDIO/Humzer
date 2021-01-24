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

		uint32_t GetColorAttachment(uint32_t index = 0) const override { HUM_ASSERT(index < m_ColorAttachments.size(), "There is no exisiting index in color attachments"); return m_ColorAttachments[index]; }

		void Resize(uint32_t width, uint32_t height) override;

	private:
		unsigned int m_ID = 0;
		/*uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;*/
		uint32_t m_IDBufferAttachment = 0;
		FramebufferSpecs m_Specs;

		std::vector<FramebufferTextureSpecs> m_ColorAttachmentsSpecs;
		FramebufferTextureSpecs m_DepthAttachmentSpecs;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};

}

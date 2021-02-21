#include "humpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Humzer {

	namespace Utils {

		static bool IsDepthFormat(FramebufferTextureFormat format) {
			switch (format)
			{
			case Humzer::FramebufferTextureFormat::DEPTH24STENCIL8:
				return true;
				break;
			}

			return false;
		}

		static GLenum TextureTarget(bool multisampled) {
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count) {
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id) {
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum accessFormat, GLenum format, uint32_t width, uint32_t height, int index) {
			bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, accessFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				
				// #TODO: Filtering
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
			bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else {
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				// #TODO: Filtering
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static GLenum HumFBTextureToGl(FramebufferTextureFormat humFormat) {

			switch (humFormat)
			{
			case Humzer::FramebufferTextureFormat::None:
				return GL_NONE;
			case Humzer::FramebufferTextureFormat::RGBA8:
				return GL_RGBA8;
			case Humzer::FramebufferTextureFormat::R32I:
				return GL_RED_INTEGER;
			case Humzer::FramebufferTextureFormat::DEPTH24STENCIL8:
				return GL_DEPTH24_STENCIL8;
			}

			HUM_ASSERT(false);
			return 0;
		}

	}

	

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& spec) : m_Specs(spec)
	{
		for (auto format : m_Specs.Attachments.Attachments) {
			if (!Utils::IsDepthFormat(format.TextureFormat))
				m_ColorAttachmentsSpecs.emplace_back(format);
			else
				m_DepthAttachmentSpecs = format;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if(m_ID){
			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		// ---------------------
		// **** ATTACHMENTS ****
		// ---------------------

		bool multisample = m_Specs.Samples > 1;

		if (m_ColorAttachmentsSpecs.size()) {
			m_ColorAttachments.resize(m_ColorAttachmentsSpecs.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentsSpecs[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specs.Samples, GL_RGBA8, GL_RGBA, m_Specs.Width, m_Specs.Height, i);
					break;
				case FramebufferTextureFormat::R32I:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specs.Samples, GL_R32I, GL_RED_INTEGER, m_Specs.Width, m_Specs.Height, i);
					break;
				}
			}
		}

		if (m_DepthAttachmentSpecs.TextureFormat != FramebufferTextureFormat::None) {
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);

			switch (m_DepthAttachmentSpecs.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specs.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specs.Width, m_Specs.Height);
				break;
			}
		}

		if (m_ColorAttachments.size() > 1) {
			HUM_ASSERT(m_ColorAttachments.size() <= 4, "Humzer only supports a max of 4 color attachments as of the moment!");

			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if(m_ColorAttachments.empty()) {
			// Only depth-pass (Useful for shadow mapping).
			glDrawBuffer(GL_NONE);
		}

		HUM_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glViewport(0, 0, m_Specs.Width, m_Specs.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) const
	{
		HUM_ASSERT(attachmentIndex < m_ColorAttachments.size());
		
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specs.Width = width;
		m_Specs.Height = height;
		Invalidate();
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t index, int value)
	{
		HUM_ASSERT(index < m_ColorAttachments.size());

		auto& spec = m_ColorAttachmentsSpecs[index];
		glClearTexImage(m_ColorAttachments[index], 0, Utils::HumFBTextureToGl(spec.TextureFormat), GL_INT, &value);
	}

}

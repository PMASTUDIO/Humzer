#pragma once
#include "../Core/Core.h"

namespace Humzer {

	enum class FramebufferTextureFormat {
		None = 0,

		// Color
		RGBA8,

		// Integers,
		R32I,
		
		// Depth
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecs {
		FramebufferTextureSpecs() = default;
		FramebufferTextureSpecs(FramebufferTextureFormat format) : TextureFormat(format){}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;

		// TODO: Filtering / Wrap
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecs> attachments) : Attachments(attachments) {}

		std::vector<FramebufferTextureSpecs> Attachments = {};
	};

	struct FramebufferSpecs {
		uint32_t Width, Height;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer {
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentId, int w, int h) const = 0;

		virtual void ClearAttachment(uint32_t index, int value) = 0;
		virtual uint32_t GetColorAttachment(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecs& GetSpecs() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecs& spec);

	};
}

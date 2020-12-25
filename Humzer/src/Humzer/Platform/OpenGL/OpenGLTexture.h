#pragma once

#include "../../Renderer/Texture.h"

namespace Humzer {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		virtual bool Loaded() const override { return m_Loaded; }

		void Bind(uint32_t slot = 0) const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		unsigned int m_ID;

		bool m_Loaded = false;
	};

}

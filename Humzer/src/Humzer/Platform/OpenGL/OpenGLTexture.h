#pragma once

#include "../../Renderer/Texture.h"
#include "glad/glad.h"

namespace Humzer {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		virtual bool Loaded() const override { return m_Loaded; }

		void Bind(uint32_t slot = 0) const override;

		void SetData(void* data, uint32_t size) override;

		bool operator ==(const Texture2D& other) const override
		{
			return m_ID == ((OpenGLTexture2D&)other).m_ID;
		}

		std::string& GetPath() override { return m_Path; }

		unsigned int GetRendererID() override { return m_ID; }

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		unsigned int m_ID;

		GLenum m_InternalFormat, m_Format;

		bool m_Loaded = false;
	};

	class OpenGLTextureCube : public TextureCube {
	public:
		OpenGLTextureCube(const std::vector<std::string> faces);
		OpenGLTextureCube(const std::string& path);

		virtual ~OpenGLTextureCube();

		const std::vector<std::string> GetFacesPath() const override { return m_Faces; }

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void Bind(uint32_t slot = 0) const override;
	private:
		const std::vector<std::string> m_Faces;

		std::string m_Path;
		uint32_t m_Width, m_Height;
		unsigned int m_ID;

		unsigned char* m_ImageData;
	};

}

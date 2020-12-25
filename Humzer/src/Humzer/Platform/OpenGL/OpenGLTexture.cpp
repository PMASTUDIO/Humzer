#include "humpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include <glad\glad.h>

namespace Humzer {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (!data) {
			HUM_CORE_ERROR("Failed to load image at {0}", path);
			return;
		}

		m_Width = width;
		m_Height = height;

		
		// UPLOAD TEXTURE TO GPU
		GLenum data_format = 0;
		GLenum internal_format = 0;
		switch (channels)
		{
		case 2:
			internal_format = GL_RG8;
			data_format = GL_RG;
			break;
		case 3:
			internal_format = GL_RGB8;
			data_format = GL_RGB;
			break;
		case 4:
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
			break;
		default:
			internal_format = GL_RGB8;
			data_format = GL_RGB;
			break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, internal_format, m_Width, m_Height);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		
		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, data_format, GL_UNSIGNED_BYTE, data);

		m_Loaded = true;

		// FREE IMAGE
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot /*= 0*/) const
	{
		glBindTextureUnit(slot, m_ID);
	}

}

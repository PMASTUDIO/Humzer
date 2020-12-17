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

		if (!data)
			HUM_CORE_ERROR("Failed to load image at {0}", path);

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, GL_RGB8, m_Width, m_Height);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// UPLOAD TEXTURE TO GPU
		GLenum image_format;
		switch (channels)
		{
		case 2:
			image_format = GL_RG;
			break;
		case 3:
			image_format = GL_RGB;
			break;
		case 4:
			image_format = GL_RGBA;
			break;
		default:
			image_format = GL_RGB;
			break;
		}

		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, image_format, GL_UNSIGNED_BYTE, data);

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

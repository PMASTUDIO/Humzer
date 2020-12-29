#include "humpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include <glad\glad.h>

namespace Humzer {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
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

	OpenGLTextureCube::OpenGLTextureCube(const std::vector<std::string> faces) : m_Width(0), m_Height(0) /*: m_Path(path)*/
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		//m_ImageData = stbi_load(path.c_str(), &width, &height, &channels, 0);

		//uint32_t faceWidth = m_Width / 4;
		//uint32_t faceHeight = m_Height / 3;

		//// #ASSERT
		//if (faceWidth != faceHeight)
		//	HUM_CORE_WARN("Cubemap textures {0} have non-square faces!", path);

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		/*GLenum data_format = 0;
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
		*/

		for (size_t i = 0; i < faces.size(); i++) {
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // 0x8515 + i
				stbi_image_free(data);
			}
			else {
				HUM_CORE_ERROR("Failed to load cube texture at: {0}", faces[i]);
				stbi_image_free(data);
			}
		}

		//// GET FACES
		//std::array<uint8_t*, 6> faces;
		//for (size_t i = 0; i < faces.size(); i++)
		//	faces[i] = new uint8_t[faceWidth * faceHeight * 3]; // 3 BPP

		//int faceIndex = 0;

		//for (size_t i = 0; i < 4; i++)
		//{
		//	for (size_t y = 0; y < faceHeight; y++)
		//	{
		//		size_t yOffset = y + faceHeight;
		//		for (size_t x = 0; x < faceWidth; x++)
		//		{
		//			size_t xOffset = x + i * faceWidth;
		//			faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
		//			faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
		//			faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
		//		}
		//	}
		//	faceIndex++;
		//}

		//for (size_t i = 0; i < 3; i++)
		//{
		//	// Skip the middle one
		//	if (i == 1)
		//		continue;

		//	for (size_t y = 0; y < faceHeight; y++)
		//	{
		//		size_t yOffset = y + i * faceHeight;
		//		for (size_t x = 0; x < faceWidth; x++)
		//		{
		//			size_t xOffset = x + faceWidth;
		//			faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
		//			faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
		//			faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
		//		}
		//	}
		//	faceIndex++;
		//}

		// GENERATE TEXTURE AND SETUP

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		/*glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internal_format, faceWidth, faceHeight, 0, internal_format, GL_UNSIGNED_BYTE, faces[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internal_format, faceWidth, faceHeight, 0, internal_format, GL_UNSIGNED_BYTE, faces[0]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internal_format, faceWidth, faceHeight, 0, internal_format, GL_UNSIGNED_BYTE, faces[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internal_format, faceWidth, faceHeight, 0, internal_format, GL_UNSIGNED_BYTE, faces[5]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internal_format, faceWidth, faceHeight, 0, internal_format, GL_UNSIGNED_BYTE, faces[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internal_format, faceWidth, faceHeight, 0, internal_format, GL_UNSIGNED_BYTE, faces[3]);*/

		//glBindTexture(GL_TEXTURE_2D, 0);

		/*for (size_t i = 0; i < faces.size(); i++)
			delete[] faces[i];*/

		//stbi_image_free(m_ImageData);
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTextureCube::Bind(uint32_t slot /*= 0*/) const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	}

}

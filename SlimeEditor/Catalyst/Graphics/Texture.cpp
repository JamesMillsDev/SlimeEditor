#include "Texture.h"

#include <glew/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Catalyst::Graphics
{
	Texture::Texture()
		: m_filename{ new char[] {"none"} }, m_width{ 0 }, m_height{ 0 }, m_glHandle{ 0 }, m_format{ 0 },
		m_loadedPixels{ nullptr }
	{
	}

	Texture::Texture(const char* _filename)
		: m_filename{ nullptr }, m_width{ 0 }, m_height{ 0 }, m_glHandle{ 0 }, m_format{ 0 },
		m_loadedPixels{ nullptr }
	{
		Load(_filename);
	}

	Texture::Texture(const uint _width, const uint _height, const EFormat _format, const byte* _pixels)
	{
		Create(_width, _height, _format, _pixels);
	}

	Texture::~Texture()
	{
		if (m_glHandle != 0)
			glDeleteTextures(1, &m_glHandle);

		if (m_loadedPixels != nullptr)
		{
			stbi_image_free(m_loadedPixels);
			m_loadedPixels = nullptr;
		}

		delete[] m_filename;
	}

	bool Texture::Load(const char* _filename)
	{
		if (m_glHandle != 0)
		{
			glDeleteTextures(1, &m_glHandle);
			m_glHandle = 0;
			m_width = 0;
			m_height = 0;
			m_filename = new char[] {"none"};
		}

		int x = 0, y = 0, comp = 0;
		m_loadedPixels = stbi_load(_filename, &x, &y, &comp, STBI_default);

		if (m_loadedPixels != nullptr)
		{
			glGenTextures(1, &m_glHandle);
			glBindTexture(GL_TEXTURE_2D, m_glHandle);
			switch (comp)
			{
			case STBI_grey:
			{
				m_format = R;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, x, y, 0, GL_RED, GL_UNSIGNED_BYTE, m_loadedPixels);
				break;
			}
			case STBI_grey_alpha:
			{
				m_format = RG;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, x, y, 0, GL_RG, GL_UNSIGNED_BYTE, m_loadedPixels);
				break;
			}
			case STBI_rgb:
			{
				m_format = RGB;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, m_loadedPixels);
				break;
			}
			case STBI_rgb_alpha:
			{
				m_format = RGBA;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_loadedPixels);
				break;
			}
			default:
			{
				break;
			}
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			m_width = static_cast<unsigned int>(x);
			m_height = static_cast<unsigned int>(y);

			m_filename = new char[strlen(_filename)];
			strcpy_s(m_filename, strlen(_filename) + 1, _filename);

			return true;
		}
		return false;
	}

	void Texture::Create(const uint _width, const uint _height, const EFormat _format, const byte* _pixels)
	{
		if (m_glHandle != 0)
		{
			glDeleteTextures(1, &m_glHandle);
			m_glHandle = 0;
			m_filename = new char[] { "none" };
		}

		m_width = _width;
		m_height = _height;
		m_format = _format;

		glGenTextures(1, &m_glHandle);
		glBindTexture(GL_TEXTURE_2D, m_glHandle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		switch (m_format)
		{
		case R:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<int>(m_width), static_cast<int>(m_height),
				0, GL_RED, GL_UNSIGNED_BYTE, _pixels);
			break;
		}
		case RG:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, static_cast<int>(m_width), static_cast<int>(m_height),
				0, GL_RG, GL_UNSIGNED_BYTE, _pixels);
			break;
		}
		case RGB:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(m_width), static_cast<int>(m_height),
				0, GL_RGB, GL_UNSIGNED_BYTE, _pixels);
			break;
		}
		case RGBA:
		default:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(m_width), static_cast<int>(m_height),
				0, GL_RGBA, GL_UNSIGNED_BYTE, _pixels);
			break;
		}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const char* Texture::GetFilename() const
	{
		return m_filename;
	}

	void Texture::Bind(const uint _slot) const
	{
		glActiveTexture(GL_TEXTURE0 + _slot);
		glBindTexture(GL_TEXTURE_2D, m_glHandle);
	}

	uint Texture::GlHandle() const
	{
		return m_glHandle;
	}

	uint Texture::Width() const
	{
		return m_width;
	}

	uint Texture::Height() const
	{
		return m_height;
	}

	uint Texture::Format() const
	{
		return m_format;
	}

	const byte* Texture::Pixels() const
	{
		return m_loadedPixels;
	}
}

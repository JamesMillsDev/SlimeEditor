#pragma once

#include "Catalyst/Types.h"

namespace Catalyst::Graphics
{
	class Texture final
	{
	public:
		enum EFormat : byte
		{
			R = 1,
			RG,
			RGB,
			RGBA
		};

	public:
		Texture();
		Texture(const char* _filename);
		Texture(uint _width, uint _height, EFormat _format, const byte* _pixels = nullptr);
		~Texture();

		Texture(const Texture&) = delete;
		Texture(Texture&&) = delete;

	public:
		// load a jpg, bmp, png or tga
		bool Load(const char* _filename);

		// creates a texture that can be filled in with pixels
		void Create(uint _width, uint _height, EFormat _format, const byte* _pixels = nullptr);

		// returns the filename or "none" if not loaded from a file
		[[nodiscard]] const char* GetFilename() const;

		// binds the texture to the specified slot
		void Bind(uint _slot) const;

		// returns the opengl texture handle
		[[nodiscard]] uint GlHandle() const;

		[[nodiscard]] uint Width() const;
		[[nodiscard]] uint Height() const;
		[[nodiscard]] uint Format() const;
		[[nodiscard]] const byte* Pixels() const;

	public:
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) = delete;

	private:
		char* m_filename;
		uint m_width;
		uint m_height;
		uint m_glHandle;
		uint m_format;
		byte* m_loadedPixels;

	};
}
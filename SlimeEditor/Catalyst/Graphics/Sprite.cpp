#include "Sprite.h"

#include "Texture.h"

namespace Catalyst::Graphics
{
	Sprite::Sprite()
		: source{ nullptr }
	{
	}

	Sprite::Sprite(Texture* _texture)
		: source{ _texture }
	{
		rect =
		{
			0,
			0,
			static_cast<float>(source->Width()),
			static_cast<float>(source->Height())
		};
	}

	Sprite::Sprite(const char* _textureName)
	{
		source = new Texture(_textureName);

		rect =
		{
			0,
			0,
			static_cast<float>(source->Width()),
			static_cast<float>(source->Height())
		};
	}

	Sprite::Sprite(const Rectangle& _rect, Texture* _texture)
		: rect{ _rect }, source{ _texture }
	{
	}

	Sprite::Sprite(const Rectangle& _rect, const char* _textureName)
		: rect{ _rect }
	{
		source = new Texture(_textureName);
	}
}

#pragma once

#include "Catalyst/Utilities/Rectangle.h"

namespace Catalyst::Graphics
{
	class Texture;

	using Utilities::Rectangle;

	class Sprite
	{
	public:
		Rectangle rect;
		Texture* source;

	public:
		Sprite();
		Sprite(Texture* _texture);
		Sprite(const char* _textureName);
		Sprite(const Rectangle& _rect, Texture* _texture);
		Sprite(const Rectangle& _rect, const char* _textureName);

	};
}

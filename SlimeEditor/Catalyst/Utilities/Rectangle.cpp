#include "Rectangle.h"

#include "Catalyst/Types.h"

namespace Catalyst::Utilities
{
	Rectangle::Rectangle()
		: x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 }
	{
	}

	Rectangle::Rectangle(float _x, float _y, float _w, float _h, float _xOrigin, float _yOrigin)
		: x{ _x }, y{ _y }, w{ _w }, h{ _h }, xOrigin{ _xOrigin }, yOrigin{ _yOrigin }
	{
	}

	Rectangle::Rectangle(const vec2& _pos, const vec2& _size, const vec2& _origin)
		: position{ _pos }, size{ _size }, origin{ _origin }
	{
	}

	Rectangle::Rectangle(float _data[RECT_ARRAY_SIZE])
	{
		for (byte i = 0; i < RECT_ARRAY_SIZE; ++i)
			data[i] = _data[i];
	}

	vec2 Rectangle::Min() const
	{
		return position - size * origin;
	}

	vec2 Rectangle::Max() const
	{
		return position + size * origin;
	}

	vec2 Rectangle::Extents() const
	{
		return { w * .5f, h * .5f };
	}
}

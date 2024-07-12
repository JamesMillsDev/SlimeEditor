#pragma once

#include <glm/vec2.hpp>

using glm::vec2;

namespace Catalyst::Utilities
{
	constexpr int RECT_ARRAY_SIZE = 6;

	/* A structure for containing simple rectangle data. Default pivot is top left */
	class Rectangle
	{
	public:
		union 
		{
			struct
			{
				float x;
				float y;
				float w;
				float h;

				float xOrigin;
				float yOrigin;
			};

			struct
			{
				vec2 position;
				vec2 size;
				vec2 origin;
			};

			float data[RECT_ARRAY_SIZE];
		};

	public:
		Rectangle();
		Rectangle(float _x, float _y, float _w, float _h, float _xOrigin = 0.f, float _yOrigin = 0.f);
		Rectangle(const vec2& _pos, const vec2& _size, const vec2& _origin);
		explicit Rectangle(float _data[RECT_ARRAY_SIZE]);

	public:
		vec2 Min() const;
		vec2 Max() const;
		vec2 Extents() const;

	};
}
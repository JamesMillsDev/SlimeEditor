#include "Color.h"

namespace Catalyst::Utilities
{
	float Color::Red() const
	{
		return static_cast<float>(r) / 255.f;
	}

	float Color::Green() const
	{
		return static_cast<float>(g) / 255.f;
	}

	float Color::Blue() const
	{
		return static_cast<float>(b) / 255.f;
	}

	float Color::Alpha() const
	{
		return static_cast<float>(a) / 255.f;
	}
}

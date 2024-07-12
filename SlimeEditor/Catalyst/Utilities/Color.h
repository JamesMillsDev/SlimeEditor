#pragma once

#include "Catalyst/Types.h"

namespace Catalyst::Utilities
{
	class Color
	{
	public:
		byte r;
		byte g;
		byte b;
		byte a;

	public:
		float Red() const;
		float Green() const;
		float Blue() const;
		float Alpha() const;

	};
}

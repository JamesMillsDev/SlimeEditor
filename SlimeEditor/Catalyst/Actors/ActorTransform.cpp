#include "ActorTransform.h"

#include <glm/glm.hpp>

namespace Catalyst::Actors
{
	ActorTransform::ActorTransform()
		: position{ 0.f, 0.f }, size{ 50.f, 50.f }, origin{ .5f, .5f }, rotation{ 0.f }
	{
	}

	ActorTransform::ActorTransform(const vec2 _position, const vec2 _size, const float _rotation)
		: position{ _position }, size{ _size }, origin{ .5f, .5f },
		rotation{ _rotation }
	{
	}

	ActorTransform::ActorTransform(const vec2 _position, const vec2 _size, const float _rotation, const vec2 _origin)
		: position{ _position }, size{ _size }, origin{ _origin }, rotation{ _rotation }
	{
	}

	ActorTransform::ActorTransform(ActorTransform&& _other) noexcept
	{
		position = _other.position;
		size = _other.size;
		origin = _other.origin;
		rotation = _other.rotation;

		_other.position = {};
		_other.size = {};
		_other.origin = {};
		_other.rotation = 0.f;
	}

	ActorTransform::~ActorTransform()
	{
		position = {};
		size = {};
		rotation = 0.f;
	}

	vec2 ActorTransform::Forward() const
	{
		const float rad = glm::radians(rotation);

		constexpr vec2 up = { 0, 1 };

		return
		{
			up.x * cosf(rad) - up.y * sinf(rad),
			up.x * sinf(rad) + up.y * cosf(rad)
		};
	}

	vec2 ActorTransform::Right() const
	{
		const float rad = glm::radians(rotation + 90.f);

		constexpr vec2 up = { 0, 1 };

		return
		{
			up.x * cosf(rad) - up.y * sinf(rad),
			up.x * sinf(rad) + up.y * cosf(rad)
		};
	}
}

#pragma once

#include <glm/vec2.hpp>

using glm::vec2;

namespace Catalyst::Actors
{
	class ActorTransform
	{
	public:
		vec2 position;
		vec2 size;
		vec2 origin;
		float rotation;

	public:
		ActorTransform();
		ActorTransform(vec2 _position, vec2 _size, float _rotation);
		ActorTransform(vec2 _position, vec2 _size, float _rotation, vec2 _origin);
		~ActorTransform();

		ActorTransform(const ActorTransform& _other) = delete;
		ActorTransform(ActorTransform&& _other) noexcept;

	public:
		[[nodiscard]] vec2 Forward() const;
		[[nodiscard]] vec2 Right() const;

	public:
		ActorTransform& operator=(const ActorTransform& _other) = delete;
		ActorTransform& operator=(ActorTransform&& _other) noexcept;

	};
}

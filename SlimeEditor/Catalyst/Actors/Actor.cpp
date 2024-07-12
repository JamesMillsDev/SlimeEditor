#include "Actor.h"

#include "ActorComponent.h"
#include "ActorTransform.h"

namespace Catalyst::Actors
{
	ActorTransform* Actor::Transform() const
	{
		return m_transform;
	}

	Actor::Actor()
		: m_transform{ new ActorTransform }
	{
	}

	Actor::Actor(const vec2 _position, const vec2 _size, const float _rotation)
		: m_transform{ new ActorTransform(_position, _size, _rotation) }
	{
	}

	Actor::~Actor()
	{
		delete m_transform;
		m_transform = nullptr;

		for (const auto& comp : m_components)
			delete comp;

		m_components.clear();
	}

	void Actor::BeginPlay() { }

	void Actor::Tick() { }

	void Actor::Render() { }

	void Actor::EndPlay() { }

	void Actor::ReceiveBeginPlay()
	{
		BeginPlay();
	}

	void Actor::ReceiveTick()
	{
		for (auto& [fnc, obj] : m_changes)
			std::invoke(fnc, this, obj);

		m_changes.clear();

		Tick();

		for (const auto& comp : m_components)
			comp->Tick();
	}

	void Actor::ReceiveRender()
	{
		Render();

		for (const auto& comp : m_components)
			comp->Render();
	}

	void Actor::ReceiveEndPlay()
	{
		EndPlay();
	}

	void Actor::AddComponent(ActorComponent* _component)
	{
		_component->BeginPlay();
		m_components.emplace_back(_component);
	}

	void Actor::RemoveComponent(ActorComponent* _component)
	{
		_component->EndPlay();
		m_components.remove(_component);
	}
}

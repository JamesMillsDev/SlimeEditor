#pragma once

#include "Catalyst/Common.h"

#include <glm/vec2.hpp>

#include <list>

using glm::vec2;

using std::list;
using std::pair;

namespace Catalyst::Actors
{
	class ActorTransform;
	class ActorComponent;

	class Actor
	{
		friend class ActorManager;

		typedef void(Actor::* ComponentListChange)(ActorComponent*);

	public:
		Actor(const Actor&) = delete;
		Actor(Actor&&) = delete;

	public:
		template<derived<ActorComponent> COMPONENT>
		COMPONENT* FindComponent();
		template<derived<ActorComponent> COMPONENT>
		COMPONENT* CreateComponent();
		template<derived<ActorComponent> COMPONENT>
		void DestroyComponent(COMPONENT* _component);

		ActorTransform* Transform() const;

	public:
		Actor& operator=(const Actor&) = delete;
		Actor& operator=(Actor&&) = delete;

	protected:
		Actor();
		Actor(vec2 _position, vec2 _size, float _rotation);
		virtual ~Actor();

	protected:
		virtual void BeginPlay();

		virtual void Tick();
		virtual void Render();

		virtual void EndPlay();

	protected:
		ActorTransform* m_transform;

	private:
		list<pair<ComponentListChange, ActorComponent*>> m_changes;
		list<ActorComponent*> m_components;

	private:
		void ReceiveBeginPlay();
		void ReceiveTick();
		void ReceiveRender();
		void ReceiveEndPlay();

		void AddComponent(ActorComponent* _component);
		void RemoveComponent(ActorComponent* _component);

	};

	template <derived<ActorComponent> COMPONENT>
	COMPONENT* Actor::FindComponent()
	{
		for (const auto& comp : m_components)
		{
			if (COMPONENT* component = dynamic_cast<COMPONENT*>(comp))
				return component;
		}

		return nullptr;
	}

	template <derived<ActorComponent> COMPONENT>
	COMPONENT* Actor::CreateComponent()
	{
		COMPONENT* component = new COMPONENT;
		component->m_owner = this;

		m_changes.emplace_back(&Actor::AddComponent, component);

		return component;
	}

	template <derived<ActorComponent> COMPONENT>
	void Actor::DestroyComponent(COMPONENT* _component)
	{
		m_changes.emplace_back(&Actor::RemoveComponent, _component);
	}
}

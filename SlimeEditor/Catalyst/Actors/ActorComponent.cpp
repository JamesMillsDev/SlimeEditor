#include "ActorComponent.h"

namespace Catalyst::Actors
{
	Actor* ActorComponent::Owner() const
	{
		return m_owner;
	}

	ActorComponent::ActorComponent()
		: m_owner{ nullptr }
	{
	}

	ActorComponent::~ActorComponent() = default;

	void ActorComponent::BeginPlay() { }

	void ActorComponent::Tick() { }

	void ActorComponent::Render() { }

	void ActorComponent::EndPlay() { }
}

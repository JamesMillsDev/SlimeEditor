#include "ActorManager.h"

#include "Actor.h"

namespace Catalyst::Actors
{
	void ActorManager::Spawn(Actor* _actor)
	{
		if (std::ranges::find(m_actors, _actor) != m_actors.end())
			return;

		m_listChanges.emplace_back(&ActorManager::AddActor, _actor);
	}

	void ActorManager::Destroy(Actor* _actor)
	{
		if (std::ranges::find(m_actors, _actor) == m_actors.end())
			return;

		m_listChanges.emplace_back(&ActorManager::RemoveActor, _actor);
	}

	ActorManager::ActorManager() = default;

	ActorManager::~ActorManager()
	{
		for (const auto& actor : m_actors)
			delete actor;

		m_actors.clear();
		m_listChanges.clear();
	}

	void ActorManager::Tick()
	{
		for (auto& [fnc, actor] : m_listChanges)
			std::invoke(fnc, this, actor);

		m_listChanges.clear();

		for (const auto& actor : m_actors)
			actor->ReceiveTick();
	}

	void ActorManager::Render() const
	{
		for (const auto& actor : m_actors)
			actor->ReceiveRender();
	}

	void ActorManager::AddActor(Actor* _actor)
	{
		_actor->ReceiveBeginPlay();
		m_actors.emplace_back(_actor);
	}

	void ActorManager::RemoveActor(Actor* _actor)
	{
		_actor->ReceiveEndPlay();
		m_actors.remove(_actor);
		delete _actor;
	}
}
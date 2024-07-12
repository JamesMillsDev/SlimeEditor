#include "Level.h"

#include "Catalyst/Actors/ActorManager.h"

namespace Catalyst::Levels
{
	const string& Level::GetName()
	{
		return m_name;
	}

	ActorManager* Level::GetActorManager() const
	{
		return m_actorManager;
	}

	Level::Level(string _name)
		: m_actorManager{ new ActorManager }, m_name{ std::move(_name) }
	{
	}

	Level::~Level()
	{
		delete m_actorManager;
		m_actorManager = nullptr;
	}

	void Level::OnLoaded() { }

	void Level::OnUnloaded() { }

	void Level::Tick() { }

	void Level::Render() { }
}

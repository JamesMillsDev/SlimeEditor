#pragma once

#include <list>

using std::list;
using std::pair;

namespace Catalyst::Levels
{
	class Level;
	class LevelManager;
}


namespace Catalyst::Actors
{
	using Levels::Level;
	using Levels::LevelManager;

	class Actor;

	class ActorManager
	{
		friend Level;
		friend LevelManager;

		typedef void(ActorManager::* ActorListChange)(Actor*);

	public:
		ActorManager(const ActorManager&) = delete;
		ActorManager(ActorManager&&) = delete;

	public:
		void Spawn(Actor* _actor);
		void Destroy(Actor* _actor);

	public:
		ActorManager& operator=(const ActorManager&) = delete;
		ActorManager& operator=(ActorManager&&) = delete;

	private:
		list<Actor*> m_actors;
		list<pair<ActorListChange, Actor*>> m_listChanges;

	private:
		ActorManager();
		~ActorManager();

	private:
		void Tick();
		void Render() const;

		void AddActor(Actor* _actor);
		void RemoveActor(Actor* _actor);

	};
}


#pragma once

#include <string>

using std::string;

namespace Catalyst::Actors
{
	class ActorManager;
}

namespace Catalyst::Levels
{
	using Actors::ActorManager;

	class Level
	{
		friend class LevelManager;

	public:
		Level(const Level&) = delete;
		Level(Level&&) = delete;

	public:
		const string& GetName();

		ActorManager* GetActorManager() const;

	public:
		Level& operator=(const Level&) = delete;
		Level& operator=(Level&&) = delete;

	protected:
		Level(string _name);
		virtual ~Level();

	protected:
		virtual void OnLoaded();
		virtual void OnUnloaded();

		virtual void Tick();
		virtual void Render();

	private:
		ActorManager* m_actorManager;
		string m_name;

	};
}

#pragma once

#include "Catalyst/Callback.h"

#include <string>
#include <list>
#include <map>

using std::string;
using std::list;
using std::map;
using std::pair;

namespace Catalyst::Levels
{
	class Level;

	class LevelManager final
	{
		friend class Application;

		typedef void(LevelManager::* LoadedLevelMapChange)(const char*);

	public:
		typedef list<Callback<Level*>*>::iterator LevelChangeIterator;

	public:
		LevelManager(const LevelManager&) = delete;
		LevelManager(LevelManager&&) = delete;

	public:
		void Load(const char* _level);
		void Unload(const char* _level);

		[[nodiscard]] bool IsLoaded(const string& _level) const;

		void AddLevel(Level* _level);
		void RemoveLevel(Level* _level);

		LevelChangeIterator ListenForLevelChange(Callback<Level*>* _onLevelChanged);
		void StopListeningToLevelChange(const LevelChangeIterator& _onLevelChanged);

	public:
		LevelManager& operator=(const LevelManager&) = delete;
		LevelManager& operator=(LevelManager&&) = delete;

	private:
		map<string, Level*> m_levels;
		Level* m_loaded;

		list<pair<LoadedLevelMapChange, const char*>> m_loadedLevelMapChanges;

		list<Callback<Level*>*> m_levelChangeListeners;

	private:
		LevelManager();
		~LevelManager();

	private:
		void Tick();
		void Render() const;

		void LoadLevel(const char* _name);
		void UnloadLevel(const char* _name);

	};
}

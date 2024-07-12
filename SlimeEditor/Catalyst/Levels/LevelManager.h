#pragma once

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
		LevelManager(const LevelManager&) = delete;
		LevelManager(LevelManager&&) = delete;

	public:
		void Load(const char* _level);
		void Unload(const char* _level);

		[[nodiscard]] bool IsLoaded(const string& _level) const;

		void AddLevel(Level* _level);
		void RemoveLevel(Level* _level);

	public:
		LevelManager& operator=(const LevelManager&) = delete;
		LevelManager& operator=(LevelManager&&) = delete;

	private:
		map<string, Level*> m_levels;
		map<string, Level*> m_loaded;

		list<pair<LoadedLevelMapChange, const char*>> m_loadedLevelMapChanges;

	private:
		LevelManager();
		~LevelManager();

	private:
		void Tick();
		void Render();

		void LoadLevel(const char* _name);
		void UnloadLevel(const char* _name);

	};
}

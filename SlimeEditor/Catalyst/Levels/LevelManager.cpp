#include "LevelManager.h"

#include <ranges>

#include "Level.h"

#include "Catalyst/Actors/ActorManager.h"

namespace Catalyst::Levels
{
	void LevelManager::Load(const char* _level)
	{
		if (IsLoaded(_level) || !m_levels.contains(_level))
			return;

		m_loadedLevelMapChanges.emplace_back(&LevelManager::LoadLevel, _level);
	}

	void LevelManager::Unload(const char* _level)
	{
		if (!IsLoaded(_level) || !m_levels.contains(_level))
			return;

		m_loadedLevelMapChanges.emplace_back(&LevelManager::UnloadLevel, _level);
	}

	bool LevelManager::IsLoaded(const string& _level) const
	{
		return m_loaded.contains(_level);
	}

	void LevelManager::AddLevel(Level* _level)
	{
		if (m_levels.contains(_level->GetName()))
			return;

		m_levels[_level->GetName()] = _level;
	}

	void LevelManager::RemoveLevel(Level* _level)
	{
		if (!m_levels.contains(_level->GetName()))
			return;

		m_levels.erase(_level->GetName());
		delete _level;
	}

	LevelManager::LevelManager() = default;

	LevelManager::~LevelManager()
	{
		// Unload all level instances and then clear the map
		for (const auto& level : m_loaded | std::views::values)
			level->OnUnloaded();

		m_loaded.clear();

		// Delete all level instances then clear the map
		for (const auto& level : m_levels | std::views::values)
			delete level;

		m_levels.clear();
	}

	void LevelManager::Tick()
	{
		// Invoke all loaded level map changes then clear the list
		for (auto& [fnc, name] : m_loadedLevelMapChanges)
			std::invoke(fnc, this, name);

		m_loadedLevelMapChanges.clear();

		// Iterate through all level instances in the loaded map and tick them
		for (const auto& level : m_loaded | std::views::values)
		{
			level->Tick();
			level->m_actorManager->Tick();
		}
	}

	void LevelManager::Render()
	{
		for (const auto& level : m_loaded | std::views::values)
		{
			level->Render();
			level->m_actorManager->Render();
		}
	}

	void LevelManager::LoadLevel(const char* _name)
	{
		m_loaded[_name] = m_levels[_name];
		m_loaded[_name]->OnLoaded();
	}

	void LevelManager::UnloadLevel(const char* _name)
	{
		m_loaded[_name]->OnUnloaded();
		m_loaded.erase(_name);
	}
}

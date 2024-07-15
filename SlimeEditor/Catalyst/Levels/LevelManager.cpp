#include "LevelManager.h"

#include <ranges>

#include "Level.h"

#include "Catalyst/Actors/ActorManager.h"
#include "Catalyst/Network/Network.h"

namespace Catalyst::Levels
{
	using Network::Network;

	void LevelManager::Load(const char* _level)
	{
		if (IsLoaded(_level) || !m_levels.contains(_level))
			return;

		if (m_loaded)
			Unload(m_loaded->GetName().c_str());

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
		if (m_loaded == nullptr)
			return false;

		return m_loaded->GetName() == _level;
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

	LevelManager::LevelChangeIterator LevelManager::ListenForLevelChange(Callback<Level*>* _onLevelChanged)
	{
		m_levelChangeListeners.emplace_back(_onLevelChanged);

		auto iter = m_levelChangeListeners.end();
		--iter;

		return iter;
	}

	void LevelManager::StopListeningToLevelChange(const LevelChangeIterator& _onLevelChanged)
	{
		if (_onLevelChanged == m_levelChangeListeners.end())
			return;

		m_levelChangeListeners.erase(_onLevelChanged);
	}

	LevelManager::LevelManager() = default;

	LevelManager::~LevelManager()
	{
		m_loaded->OnUnloaded();

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

		if (m_loaded)
		{
			m_loaded->Tick();
			m_loaded->m_actorManager->Tick();
		}
	}

	void LevelManager::Render() const
	{
		if (m_loaded)
		{
			m_loaded->Render();
			m_loaded->m_actorManager->Render();
		}
	}

	void LevelManager::LoadLevel(const char* _name)
	{
		m_loaded = m_levels[_name];
		m_loaded->OnLoaded();

		for (auto& listener : m_levelChangeListeners)
			listener->Invoke(m_loaded);
	}

	void LevelManager::UnloadLevel(const char* _name)
	{
		m_loaded->OnUnloaded();
		m_loaded = nullptr;
	}
}

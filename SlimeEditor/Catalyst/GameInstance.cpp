#include "GameInstance.h"

namespace Catalyst
{
	void GameInstance::BeginPlay() { }

	void GameInstance::Tick() { }

	void GameInstance::Render() { }

	void GameInstance::EndPlay() { }

	GameInstance::GameInstance()
		: m_screen{ nullptr }, m_levelManager{ nullptr }
	{
	}

	GameInstance::~GameInstance() = default;
}

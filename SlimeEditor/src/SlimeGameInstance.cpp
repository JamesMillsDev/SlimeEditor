#include "SlimeGameInstance.h"

#include "Catalyst/Application.h"
#include "Catalyst/Input.h"
#include "Catalyst/Levels/LevelManager.h"
#include "Levels/EditorLevel.h"

using Catalyst::Application;

SlimeGameInstance::SlimeGameInstance(const bool _isServer)
	: m_network{ new Network(_isServer) }
{
}

void SlimeGameInstance::BeginPlay()
{
	GameInstance::BeginPlay();

	if (!m_network->Initialize())
		Application::Quit();

	m_levelManager->AddLevel(new EditorLevel);
	m_levelManager->Load(EDITOR_LEVEL_NAME);
}

void SlimeGameInstance::Tick()
{
	GameInstance::Tick();

	m_network->Tick();
}

void SlimeGameInstance::Render()
{
	GameInstance::Render();
}

void SlimeGameInstance::EndPlay()
{
	GameInstance::EndPlay();

	m_network->Shutdown();
}

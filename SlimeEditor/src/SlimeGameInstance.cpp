#include "SlimeGameInstance.h"

#include "Catalyst/Application.h"
#include "Catalyst/Input.h"
#include "Catalyst/Levels/LevelManager.h"
#include "Levels/EditorLevel.h"
#include "Network/TestNetworkActor.h"

using Catalyst::Application;

using Catalyst::Network::Packet;

SlimeGameInstance::SlimeGameInstance(const bool _isServer)
	: m_network{ nullptr }, m_isServer{ _isServer }
{
}

void SlimeGameInstance::BeginPlay()
{
	GameInstance::BeginPlay();

	m_network = new Network(m_isServer, m_levelManager);

	if (!m_network->Initialize())
		Application::Quit();

	m_network->AssignPlayerPrototype(new TestNetworkActor);

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

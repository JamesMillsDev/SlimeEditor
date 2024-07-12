#include "EditorLevel.h"

#include "Catalyst/Actors/ActorManager.h"
#include "Catalyst/Network/Network.h"
#include "Network/TestNetworkActor.h"

using Catalyst::Network::Network;

EditorLevel::EditorLevel()
	: Level(EDITOR_LEVEL_NAME), m_testNetworkActor{ nullptr }
{
}

void EditorLevel::OnLoaded()
{
	if (!Network::Instance()->IsServer())
	{
		m_testNetworkActor = new TestNetworkActor;

		GetActorManager()->Spawn(m_testNetworkActor);
	}
}

void EditorLevel::OnUnloaded()
{
	Level::OnUnloaded();

	if (!Network::Instance()->IsServer())
	{
		GetActorManager()->Destroy(m_testNetworkActor);
	}
}

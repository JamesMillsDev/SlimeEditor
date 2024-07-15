#include "EditorLevel.h"

#include "Catalyst/Network/Network.h"
#include "Network/TestNetworkActor.h"

using Catalyst::Network::Network;

EditorLevel::EditorLevel()
	: Level(EDITOR_LEVEL_NAME), m_testNetworkActor{ nullptr }
{
}

void EditorLevel::OnLoaded()
{
	
}

void EditorLevel::OnUnloaded()
{
	Level::OnUnloaded();
}

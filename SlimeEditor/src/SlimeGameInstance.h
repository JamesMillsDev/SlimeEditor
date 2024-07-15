#pragma once
#include "Catalyst/GameInstance.h"

#include "Catalyst/Network/Network.h"

using Catalyst::GameInstance;
using Catalyst::Network::Network;

class SlimeGameInstance final : public GameInstance
{
public:
	SlimeGameInstance(bool _isServer);

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void EndPlay() override;

private:
	Network* m_network;

	bool m_isServer;

};
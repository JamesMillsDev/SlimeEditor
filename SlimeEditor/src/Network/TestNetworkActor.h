#pragma once

#include "Catalyst/Graphics/SpriteRenderer.h"
#include "Catalyst/Network/Actors/NetworkPlayer.h"
#include "Catalyst/Network/Actors/NetworkActor.h"

using Catalyst::Network::NetworkActor;
using Catalyst::Network::NetworkPlayer;
using Catalyst::Graphics::SpriteRenderer;

class TestNetworkActor final : public NetworkPlayer
{
public:
	TestNetworkActor();

public:
	void BeginPlay() override;

public:
	NetworkPlayer* Clone() override;

private:
	SpriteRenderer* m_renderer;

};
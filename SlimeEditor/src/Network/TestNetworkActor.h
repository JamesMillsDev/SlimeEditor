#pragma once

#include "Catalyst/Graphics/SpriteRenderer.h"
#include "Catalyst/Network/Actors/NetworkActor.h"

using Catalyst::Network::NetworkActor;
using Catalyst::Graphics::SpriteRenderer;

class TestNetworkActor final : public NetworkActor
{
public:
	TestNetworkActor();

private:
	SpriteRenderer* m_renderer;

};
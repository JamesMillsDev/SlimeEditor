#include "TestNetworkActor.h"

#include "Catalyst/Graphics/Sprite.h"

using Catalyst::Graphics::Sprite;

TestNetworkActor::TestNetworkActor()
{
	Grant();

	m_renderer = CreateComponent<SpriteRenderer>();

	m_renderer->SetSprite(new Sprite("assets/crate.png"));
}

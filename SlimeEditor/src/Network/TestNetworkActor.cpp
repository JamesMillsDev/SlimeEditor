#include "TestNetworkActor.h"

#include "Catalyst/Actors/ActorTransform.h"
#include "Catalyst/Graphics/Sprite.h"

using Catalyst::Graphics::Sprite;

TestNetworkActor::TestNetworkActor()
	: NetworkPlayer()
{
	Grant();

	m_renderer = CreateComponent<SpriteRenderer>();

	m_renderer->SetSprite(new Sprite("assets/crate.png"));
}

void TestNetworkActor::BeginPlay()
{
	NetworkPlayer::BeginPlay();

	m_transform->position =
	{
		rand() % 255,
		rand() % 255
	};
}

NetworkPlayer* TestNetworkActor::Clone()
{
	return new TestNetworkActor();
}

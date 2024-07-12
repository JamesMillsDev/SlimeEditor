#pragma once

#include "Catalyst/Actors/ActorComponent.h"
#include "Catalyst/Actors/ActorTransform.h"
#include "Catalyst/Network/IAuthorityProvider.h"
#include "Catalyst/Network/Packets/IPacketHandler.h"

namespace Catalyst::Network
{
	using Actors::ActorTransform;
	using Actors::ActorComponent;

	constexpr enet_uint8 TRANSFORM_PACKET_ID = 255;

	class NetworkTransform final : public ActorComponent, public IPacketHandler, public IAuthorityProvider
	{
	public:
		NetworkTransform();

	public:
		void Read(Packet* _packet) override;
		void Handle() override;

		void SetSyncInterval(float _interval);

	protected:
		void Tick() override;

	private:
		vec2 m_pos;
		vec2 m_size;
		float m_rotation;

		vec2 m_origin;

		float m_syncTime;
		float m_syncInterval;

	};
}

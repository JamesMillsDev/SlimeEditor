#pragma once

#include "Catalyst/Types.h"
#include "Catalyst/Actors/ActorComponent.h"
#include "Catalyst/Actors/ActorTransform.h"
#include "Catalyst/Network/IAuthorityProvider.h"
#include "Catalyst/Network/Packets/IPacketHandler.h"

namespace Catalyst::Network
{
	using Actors::ActorTransform;
	using Actors::ActorComponent;

	class NetworkActor;

	class NetworkTransform final : public ActorComponent, public IPacketHandler, public IAuthorityProvider
	{
	public:
		NetworkTransform();

	public:
		bool Read(Packet* _packet) override;
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

		ushort m_targetId;

	private:
		NetworkActor* NetOwner() const;

	};
}

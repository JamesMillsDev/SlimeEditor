#pragma once
#include "IAuthorityProvider.h"
#include "Catalyst/Types.h"
#include "Catalyst/Actors/ActorComponent.h"
#include "Packets/IPacketHandler.h"

namespace Catalyst::Network
{
	using Actors::ActorComponent;

	class NetworkId final : public ActorComponent, public IPacketHandler, public IAuthorityProvider
	{
		friend class Network;

	public:
		NetworkId();

	public:
		bool Read(Packet* _packet) override;
		void Handle() override;

		[[nodiscard]] ushort NetId() const;

	public:
		operator ushort() const;

	private:
		ushort m_netId;

	};
}

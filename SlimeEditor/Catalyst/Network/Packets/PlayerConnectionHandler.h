#pragma once
#include "IPacketHandler.h"
#include "Catalyst/Types.h"

namespace Catalyst::Network
{
	class NetworkPlayer;
	
	class PlayerConnectionHandler final : public IPacketHandler
	{
		friend class Network;

	public:
		PlayerConnectionHandler();

	public:
		bool Read(Packet* _packet) override;
		void Handle() override;

		void AssignPlayerPrototype(NetworkPlayer* _actor);

	private:
		NetworkPlayer* m_playerPrototype;
		byte m_playerId;
		ushort m_netId;

	};
}

#include "PlayerConnectionHandler.h"

#include "Packet.h"
#include "Catalyst/Network/GlobalPacketIDs.h"
#include "Catalyst/Network/Network.h"
#include "Catalyst/Network/Actors/NetworkPlayer.h"

namespace Catalyst::Network
{
	PlayerConnectionHandler::PlayerConnectionHandler()
		: IPacketHandler(PlayerConnection), m_playerPrototype{ nullptr }, m_playerId{ 255 }, m_netId{ USHRT_MAX }
	{
	}

	bool PlayerConnectionHandler::Read(Packet* _packet)
	{
		m_playerId = _packet->ReadByte();
		m_netId = _packet->ReadShort();

		return true;
	}

	void PlayerConnectionHandler::Handle()
	{
		if (!m_playerPrototype || m_playerId == 255)
			return;

		Network::Instance()->RegisterPlayer(m_playerPrototype->Clone(), m_playerId, m_netId);
	}

	void PlayerConnectionHandler::AssignPlayerPrototype(NetworkPlayer* _actor)
	{
		m_playerPrototype = _actor;
	}
}

#include "NetworkId.h"

#include <climits>

#include "GlobalPacketIDs.h"
#include "Catalyst/Network/Packets/Packet.h"

namespace Catalyst::Network
{
	NetworkId::NetworkId()
		: IPacketHandler(EGlobalPacketId::NetworkId), m_netId{ USHRT_MAX }
	{
	}

	bool NetworkId::Read(Packet* _packet)
	{
		m_netId = _packet->ReadShort();
		return true;
	}

	void NetworkId::Handle()
	{
	}

	ushort NetworkId::NetId() const
	{
		return m_netId;
	}

	NetworkId::operator unsigned short() const
	{
		return m_netId;
	}
}

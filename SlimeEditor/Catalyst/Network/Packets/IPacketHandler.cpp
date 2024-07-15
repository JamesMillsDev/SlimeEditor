#include "IPacketHandler.h"

namespace Catalyst::Network
{
	IPacketHandler::IPacketHandler(const enet_uint8 _id)
		: m_packetId{ _id }
	{
	}

	enet_uint8 IPacketHandler::PacketId() const
	{
		return m_packetId;
	}
}
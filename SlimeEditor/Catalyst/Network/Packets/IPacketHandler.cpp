#include "IPacketHandler.h"

namespace Catalyst::Network
{
	IPacketHandler::IPacketHandler(const enet_uint8 _id)
		: m_id{ _id }
	{
	}

	enet_uint8 IPacketHandler::Id() const
	{
		return m_id;
	}
}
#include "NetId.h"

namespace Catalyst::Network
{
	NetId::NetId(const byte _id)
		: m_id{ _id }
	{
	}

	byte NetId::Id() const
	{
		return m_id;
	}

	NetId::operator byte() const
	{
		return m_id;
	}
}

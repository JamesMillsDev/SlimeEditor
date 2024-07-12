#include "INetworkPlayer.h"

namespace Catalyst::Network
{
	INetworkPlayer::INetworkPlayer()
		: m_id{ 255 }
	{
	}

	byte INetworkPlayer::Id() const
	{
		return m_id;
	}
}

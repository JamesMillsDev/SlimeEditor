#include "NetworkPlayer.h"

namespace Catalyst::Network
{
	NetworkPlayer::NetworkPlayer()
		: NetworkActor(), m_playerId{ 255 }
	{
	}

	byte NetworkPlayer::PlayerId() const
	{
		return m_playerId;
	}
}

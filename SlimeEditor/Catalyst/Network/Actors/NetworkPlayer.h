#pragma once
#include "NetworkActor.h"
#include "Catalyst/Types.h"

namespace Catalyst::Network
{
	class NetworkPlayer : public NetworkActor
	{
		friend class Network;
		friend class PlayerConnectionHandler;
		
	public:
		NetworkPlayer();
		~NetworkPlayer() override = default;

	public:
		[[nodiscard]] byte PlayerId() const;

		virtual NetworkPlayer* Clone() = 0;

	private:
		byte m_playerId;

	};
}

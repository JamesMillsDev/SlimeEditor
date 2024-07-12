#pragma once
#include "Catalyst/Types.h"

namespace Catalyst::Network
{
	class INetworkPlayer
	{
		friend class Network;
		
	public:
		INetworkPlayer();

	public:
		[[nodiscard]] byte Id() const;

	private:
		byte m_id;

	};
}

#pragma once

#include "NetworkActor.h"

namespace Catalyst::Network
{
	class Packet;

	class NetworkSpawnable : public NetworkActor
	{
		friend class NetworkActorManager;

	private:
		NetworkSpawnable* SpawnWith(Packet* _packet);

	};
}

#pragma once

#include <enet/enet.h>

#include "INetworkSide.h"

namespace Catalyst::Network
{
	class NetworkServer final : public INetworkSide
	{
	public:
		NetworkServer(size_t _maxClients, size_t _channels, enet_uint32 _bandwidthOut, enet_uint32 _bandwidthIn);
		~NetworkServer() override;

		NetworkServer(const NetworkServer&) = delete;
		NetworkServer(NetworkServer&&) = delete;

	public:
		bool Start() override;
		void Stop() const override;
		void Tick() override;
		bool IsServer() override;

	public:
		NetworkServer& operator=(const NetworkServer&) = delete;
		NetworkServer& operator=(NetworkServer&&) = delete;

	private:
		size_t m_maxClients;

	};
}

#pragma once

#include <enet/enet.h>

#include "INetworkSide.h"

namespace Catalyst::Network
{
	class NetworkClient final : public INetworkSide
	{
	public:
		NetworkClient(size_t _maxConnections, size_t _channels, enet_uint32 _bandwidthOut, enet_uint32 _bandwidthIn);
		~NetworkClient() override;

		NetworkClient(const NetworkClient&) = delete;
		NetworkClient(NetworkClient&&) = delete;

	public:
		bool Start() override;
		void Stop() const override;
		void Tick() override;

		bool IsServer() override;

	public:
		NetworkClient& operator=(const NetworkClient&) = delete;
		NetworkClient& operator=(NetworkClient&&) = delete;

	private:
		size_t m_maxConnections;

		ENetPeer* m_client;
		
	};
}

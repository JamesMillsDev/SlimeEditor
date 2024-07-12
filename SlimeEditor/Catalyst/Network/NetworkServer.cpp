#include "NetworkServer.h"

#include <cstdio>

namespace Catalyst::Network
{
	NetworkServer::NetworkServer(const size_t _maxClients, const size_t _channels, 
		const enet_uint32 _bandwidthOut, const enet_uint32 _bandwidthIn)
		: INetworkSide(_channels, _bandwidthOut, _bandwidthIn), m_maxClients{ _maxClients }
	{
	}

	NetworkServer::~NetworkServer() = default;

	bool NetworkServer::Start()
	{
		m_address = { 0, 7774 };
		enet_address_set_host(&m_address, "localhost"); // TODO: move away from localhost
		m_host = enet_host_create(&m_address, m_maxClients, m_channels, m_bandwidthIn, m_bandwidthOut);

		if(m_host == nullptr)
		{
			(void)fprintf(stderr, "An error occurred while trying to create an ENet servers host.\n");
			return false;
		}

		return true;
	}

	void NetworkServer::Stop() const
	{
		enet_host_destroy(m_host);
	}

	void NetworkServer::Tick()
	{
		
	}

	bool NetworkServer::IsServer()
	{
		return true;
	}
}

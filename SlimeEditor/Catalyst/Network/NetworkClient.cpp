#include "NetworkClient.h"

#include <cstdio>

namespace Catalyst::Network
{
	NetworkClient::NetworkClient(const size_t _maxConnections, const size_t _channels,
		const enet_uint32 _bandwidthOut, const enet_uint32 _bandwidthIn)
		: INetworkSide(_channels, _bandwidthOut, _bandwidthIn), m_maxConnections{ _maxConnections }, m_client{ nullptr }
	{
	}

	NetworkClient::~NetworkClient() = default;

	bool NetworkClient::Start()
	{
		m_host = enet_host_create(nullptr, m_maxConnections, m_channels, m_bandwidthIn, m_bandwidthOut);

		if(m_host == nullptr)
		{
			(void)fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
			return false;
		}

		m_address = { 0, 7774 };
		(void)enet_address_set_host(&m_address, "localhost"); // TODO: move away from localhost

		m_client = enet_host_connect(m_host, &m_address, m_channels, 0);
		ENetEvent event;

		/* Wait up to 5 seconds for the connection attempt to succeed. */
		if (enet_host_service(m_host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			puts("Connection to localhost:7774 succeeded.");
		}
		else
		{
			/* Either the 5 seconds are up or a disconnect event was */
			/* received. Reset the peer in the event the 5 seconds   */
			/* had run out without any significant event.            */
			enet_peer_reset(m_client);

			puts("Connection to localhost:7774 failed.");
		}

		return true;
	}

	void NetworkClient::Stop() const
	{
		enet_host_destroy(m_host);
	}

	void NetworkClient::Tick()
	{
	}

	bool NetworkClient::IsServer()
	{
		return false;
	}
}

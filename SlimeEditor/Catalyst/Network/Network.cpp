#include "Network.h"

#include <cassert>
#include <cstdio>
#include <ranges>
#include <enet/enet.h>

#include "NetworkClient.h"
#include "NetworkServer.h"
#include "Catalyst/Common.h"
#include "Packets/IPacketHandler.h"
#include "Packets/Packet.h"

namespace Catalyst::Network
{
	Network* Network::m_instance = nullptr;

	Network::Network(const bool _isServer)
	{
		m_instance = this;

		if (_isServer)
			m_networkSide = new NetworkServer(4, 2, 0, 0);
		else
			m_networkSide = new NetworkClient(1, 2, 0, 0);
	}

	Network::~Network()
	{
		delete m_networkSide;
	}

	Network* Network::Instance()
	{
		return m_instance;
	}

	bool Network::Initialize()
	{
		if (enet_initialize() != 0)
			return false;

		(void)atexit(enet_deinitialize);

		if (m_networkSide)
			m_networkSide->Start();

		if (IsServer())
		{
			for (ushort i = 0; i < USHRT_MAX - 1; ++i)
				m_availableNetId.push(i);

			for (byte i = 0; i < MAX_PLAYER_COUNT; ++i)
				m_availablePlayerId.emplace_back(i);
		}

		return true;
	}

	void Network::Tick()
	{
		if (m_networkSide)
		{
			ENetEvent event;

			while (enet_host_service(m_networkSide->Host(), &event, 5) > 0)
			{
				switch (event.type)
				{
				case ENET_EVENT_TYPE_CONNECT:
				{
					printf("A new client connected from %x:%u.\n",
						event.peer->address.host,
						event.peer->address.port);

					break;
				}

				case ENET_EVENT_TYPE_RECEIVE:
				{
					Packet* packet = new Packet(event.packet);

					if (m_handlers.contains(packet->Id()))
					{
						for (const auto& handler : m_handlers[packet->Id()])
						{
							handler->Read(packet);
							handler->Handle();
						}
					}

					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy(event.packet);

					delete packet;

					break;
				}

				case ENET_EVENT_TYPE_DISCONNECT:
				{
					printf("%p disconnected.\n", event.peer->data);

					/* Reset the peer's client information. */

					event.peer->data = nullptr;
					break;
				}

				case ENET_EVENT_TYPE_NONE:
				{
					printf("Invalid event type.\n");
					break;
				}
				}
			}

			if (m_networkSide->Ready())
				m_networkSide->Tick();
		}
	}

	void Network::Shutdown()
	{
		for (const auto& group : m_handlers | std::views::values)
		{
			while (!group.empty())
				Deregister(group.front());
		}

		if (m_networkSide)
			m_networkSide->Stop();
	}

	void Network::Register(IPacketHandler* _handler)
	{
		if (!m_handlers.contains(_handler->Id()))
			m_handlers[_handler->Id()] = list<IPacketHandler*>();

		m_handlers[_handler->Id()].emplace_back(_handler);
	}

	void Network::Deregister(IPacketHandler* _handler)
	{
		if (m_handlers.contains(_handler->Id()))
		{
			list<IPacketHandler*>& handlerGroup = m_handlers[_handler->Id()];

			auto iter = std::ranges::find(handlerGroup, _handler);
			if (iter != handlerGroup.end())
				handlerGroup.remove(_handler);
		}
	}

	void Network::Send(const Packet* _packet, const enet_uint8 _channel) const
	{
		assert(m_networkSide);

		_packet->Broadcast(m_networkSide, _channel);
	}

	bool Network::IsServer() const
	{
		assert(m_networkSide);

		return m_networkSide->IsServer();
	}
}

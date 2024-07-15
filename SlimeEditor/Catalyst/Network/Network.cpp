#include "Network.h"

#include <cassert>
#include <cstdio>
#include <format>
#include <ranges>
#include <enet/enet.h>

#include "GlobalPacketIDs.h"
#include "NetworkClient.h"
#include "NetworkId.h"
#include "NetworkServer.h"
#include "Actors/NetworkActorManager.h"
#include "Actors/NetworkPlayer.h"
#include "Actors/NetworkSpawnable.h"
#include "Packets/IPacketHandler.h"
#include "Packets/Packet.h"

namespace Catalyst::Network
{
	Network* Network::m_instance = nullptr;

	Network::Network(const bool _isServer, LevelManager* _levelManager)
	{
		m_connectionHandler = new PlayerConnectionHandler;
		m_networkActorManager = new NetworkActorManager;

		m_networkActorManager->UseLevelManager(_levelManager);

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

		Register(m_connectionHandler);

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
						if (IsServer())
							CreatePlayerConnection(&event);
					}

					break;

				case ENET_EVENT_TYPE_RECEIVE:
					{
						Packet* packet = new Packet(event.packet);

						if (m_handlers.contains(packet->Id()))
						{
							for (const auto& handler : m_handlers[packet->Id()])
							{
								if(handler->Read(packet))
								{
									handler->Handle();
									break;
								}
							}
						}

						/* Clean up the packet now that we're done using it. */
						enet_packet_destroy(event.packet);

						delete packet;
					}

					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					{
						if (IsServer())
						{
							for (auto& [id, peer] : m_connectedPeers)
							{
								if (peer == event.peer)
								{
									m_availablePlayerId.emplace_front(id);

									m_connectedPeers.erase(m_connectedPeers.find(id));
								}
							}
						}

						/* Reset the peer's client information. */

						event.peer->data = nullptr;
					}
					break;

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
		if (!m_handlers.contains(_handler->PacketId()))
			m_handlers[_handler->PacketId()] = list<IPacketHandler*>();

		m_handlers[_handler->PacketId()].emplace_back(_handler);
	}

	void Network::Deregister(IPacketHandler* _handler)
	{
		if (m_handlers.contains(_handler->PacketId()))
		{
			list<IPacketHandler*>& handlerGroup = m_handlers[_handler->PacketId()];

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

	void Network::AssignPlayerPrototype(NetworkPlayer* _playerPrototype) const
	{
		m_connectionHandler->AssignPlayerPrototype(_playerPrototype);
	}

	void Network::RegisterPlayer(NetworkPlayer* _networkPlayer, byte _id, const ushort _netId)
	{
		if (m_spawnedPlayers.contains(_id))
		{
			delete _networkPlayer;
			throw std::runtime_error(std::format("PlayerID {} already used!", _id));
		}

		_networkPlayer->m_playerId = _id;
		_networkPlayer->m_id->m_netId = _netId;
		m_spawnedPlayers.emplace(_id, _networkPlayer);

		if (m_networkActorManager && m_networkActorManager->m_currentActorManager)
			m_networkActorManager->m_currentActorManager->Spawn(_networkPlayer);
	}

	void Network::SendTo(ENetPeer* _peer, const Packet* _packet, const enet_uint8 _channel) const
	{
		assert(m_networkSide);

		enet_peer_send(_peer, _channel, _packet->m_packet);
	}

	void Network::CreatePlayerConnection(ENetEvent* _event)
	{
		// Track the connected players
		const byte playerId = m_availablePlayerId.front();
		m_availablePlayerId.pop_front();

		const ushort netId = m_availableNetId.front();

		Packet* packet = new Packet(PlayerConnection);
		packet->Write(playerId);
		packet->Write(netId);

		Send(packet);

		if (m_networkActorManager && m_networkActorManager->m_currentActorManager)
		{
			NetworkPlayer* newPlayer = m_connectionHandler->m_playerPrototype->Clone();
			newPlayer->m_playerId = playerId;
			newPlayer->m_id->m_netId = netId;

			m_networkActorManager->m_currentActorManager->Spawn(newPlayer);

			m_spawnedPlayers[playerId] = newPlayer;
		}

		delete packet;

		for (const auto& id : m_connectedPeers | std::views::keys)
		{
			packet = new Packet(PlayerConnection);
			packet->Write(id);
			packet->Write(m_spawnedPlayers[id]->Id()->NetId());

			SendTo(_event->peer, packet);

			delete packet;
		}

		m_connectedPeers.emplace(playerId, _event->peer);
	}
}

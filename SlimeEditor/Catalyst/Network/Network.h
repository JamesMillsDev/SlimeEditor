#pragma once

#include <list>
#include <map>
#include <queue>
#include <enet/types.h>

#include "Catalyst/Types.h"
#include "Packets/Packet.h"
#include "Packets/PlayerConnectionHandler.h"

using std::queue;
using std::list;
using std::map;

namespace Catalyst::Levels
{
	class LevelManager;
}

namespace Catalyst::Network
{
	using Levels::LevelManager;

	class INetworkSide;
	class IPacketHandler;
	class Packet;
	class NetworkPlayer;
	class NetworkActorManager;

	/* 255 is reserved for invalid and 254 is reserved for the server */
	constexpr byte MAX_PLAYER_COUNT = 250;

	class Network
	{
		friend LevelManager;
		friend class PlayerConnectionHandler;

	public:
		Network() = delete;
		explicit Network(bool _isServer, LevelManager* _levelManager);
		~Network();

		Network(const Network&) = delete;
		Network(Network&&) = delete;

	public:
		static Network* Instance();

	public:
		[[nodiscard]] bool Initialize();
		void Tick();
		void Shutdown();

		void Register(IPacketHandler* _handler);
		void Deregister(IPacketHandler* _handler);

		void Send(const Packet* _packet, enet_uint8 _channel = 0) const;

		[[nodiscard]] bool IsServer() const;

		void AssignPlayerPrototype(NetworkPlayer* _playerPrototype) const;

	public:
		Network& operator=(const Network&) = delete;
		Network& operator=(Network&&) = delete;

	private:
		static Network* m_instance;

	private:
		/* All currently valid playerIds, the one at the front is the next one to be used when a player connects. */
		list<byte> m_availablePlayerId;
		/* All available network ids. This gets refreshed everytime an object gets destroyed. Treated like an object pool. */
		queue<ushort> m_availableNetId;

		INetworkSide* m_networkSide;

		// first = Player ID, second = connection
		map<byte, ENetPeer*> m_connectedPeers;
		map<byte, list<IPacketHandler*>> m_handlers;
		map<byte, NetworkPlayer*> m_spawnedPlayers;

		PlayerConnectionHandler* m_connectionHandler;
		NetworkActorManager* m_networkActorManager;

	private:
		void RegisterPlayer(NetworkPlayer* _networkPlayer, byte _id, ushort _netId);

		void SendTo(ENetPeer* _peer, const Packet* _packet, enet_uint8 _channel = 0) const;

		void CreatePlayerConnection(ENetEvent* _event);

	};
}

#pragma once

#include <list>
#include <map>
#include <queue>
#include <enet/types.h>

#include "Catalyst/Types.h"
#include "Packets/Packet.h"

using std::queue;
using std::list;
using std::map;

namespace Catalyst::Network
{
	class INetworkSide;
	class IPacketHandler;
	class Packet;

	/* 255 is reserved for invalid and 254 is reserved for the server */
	constexpr byte MAX_PLAYER_COUNT = 250;

	class Network
	{
	public:
		Network() = delete;
		explicit Network(bool _isServer);
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

	public:
		Network& operator=(const Network&) = delete;
		Network& operator=(Network&&) = delete;

	private:
		static Network* m_instance;

		/* All currently valid playerIds, the one at the front is the next one to be used when a player connects. */
		list<byte> m_availablePlayerId;
		/* All available network ids. This gets refreshed everytime an object gets destroyed. Treated like an object pool. */
		queue<ushort> m_availableNetId;

	private:
		INetworkSide* m_networkSide;

		map<enet_uint8, list<IPacketHandler*>> m_handlers;

	};
}

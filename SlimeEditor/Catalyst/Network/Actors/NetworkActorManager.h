#pragma once

#include "Catalyst/Levels/Level.h"
#include "Catalyst/Levels/LevelManager.h"
#include "Catalyst/Actors/ActorManager.h"
#include "Catalyst/Network/Packets/IPacketHandler.h"

#include <vector>

using std::vector;

namespace Catalyst::Network
{
	using Levels::Level;
	using Levels::LevelManager;

	using Actors::ActorManager;

	class Network;
	class NetworkSpawnable;

	class NetworkActorManager final : public IPacketHandler
	{
		friend Network;

	public:
		NetworkActorManager();

	public:
		void SpawnForAll(const Network* _network, NetworkSpawnable* _spawnable);
		void RegisterNetworkSpawnable(NetworkSpawnable* _spawnable);
		void DeregisterNetworkSpawnable(NetworkSpawnable* _spawnable);

		void UseLevelManager(LevelManager* _levelManager);

		bool Read(Packet* _packet) override;
		void Handle() override;

	private:
		vector<NetworkSpawnable*> m_spawnables;
		Packet* m_pendingActorPacket;

		ActorManager* m_currentActorManager;

		LevelManager::LevelChangeIterator m_changeIterator;

	private:
		int IndexOf(const NetworkSpawnable* _spawnable) const;

		void OnLevelChanged(Level* _level);

	};
}

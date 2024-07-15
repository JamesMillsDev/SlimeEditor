#include "NetworkActorManager.h"

#include "NetworkSpawnable.h"
#include "Catalyst/Network/GlobalPacketIDs.h"
#include "Catalyst/Network/Network.h"
#include "Catalyst/Network/Packets/Packet.h"

namespace Catalyst::Network
{
	NetworkActorManager::NetworkActorManager()
		: IPacketHandler(ActorSpawn), m_pendingActorPacket{ nullptr }, m_currentActorManager{ nullptr }
	{

	}

	void NetworkActorManager::SpawnForAll(const Network* _network, NetworkSpawnable* _spawnable)
	{
		auto iter = std::ranges::find(m_spawnables, _spawnable);
		if (iter == m_spawnables.end())
			return;

		int index = IndexOf(_spawnable);
		if (index == -1)
			return;

		Packet* spawnPacket = new Packet(ActorSpawn);
		spawnPacket->Write(index);

		_network->Send(spawnPacket);

		delete spawnPacket;
	}

	void NetworkActorManager::RegisterNetworkSpawnable(NetworkSpawnable* _spawnable)
	{
		if (std::ranges::find(m_spawnables, _spawnable) != m_spawnables.end())
			return;

		m_spawnables.emplace_back(_spawnable);
	}

	void NetworkActorManager::DeregisterNetworkSpawnable(NetworkSpawnable* _spawnable)
	{
		const auto iter = std::ranges::find(m_spawnables, _spawnable);

		if (iter == m_spawnables.end())
			return;

		m_spawnables.erase(iter);
	}

	void NetworkActorManager::UseLevelManager(LevelManager* _levelManager)
	{
		m_changeIterator = _levelManager->ListenForLevelChange(
			new VaradicTemplateCallback(this, &NetworkActorManager::OnLevelChanged)
		);
	}

	bool NetworkActorManager::Read(Packet* _packet)
	{
		m_pendingActorPacket = _packet;

		return true;
	}

	void NetworkActorManager::Handle()
	{
		int index = m_pendingActorPacket->ReadInt();

		if(index < static_cast<int>(m_spawnables.size()))
			m_currentActorManager->Spawn(m_spawnables[index]->SpawnWith(m_pendingActorPacket));
	}

	int NetworkActorManager::IndexOf(const NetworkSpawnable* _spawnable) const
	{
		for(int i = 0; i < static_cast<int>(m_spawnables.size()); ++i)
		{
			if (_spawnable == m_spawnables[i])
				return i;
		}

		return -1;
	}

	void NetworkActorManager::OnLevelChanged(Level* _level)
	{
		m_currentActorManager = _level->GetActorManager();
	}
}

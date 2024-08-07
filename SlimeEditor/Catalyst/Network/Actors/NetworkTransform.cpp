#include "NetworkTransform.h"

#include "NetworkActor.h"
#include "Catalyst/Actors/Actor.h"
#include "Catalyst/Network/GlobalPacketIDs.h"
#include "Catalyst/Network/Network.h"
#include "Catalyst/Network/NetworkId.h"
#include "Catalyst/Network/Packets/Packet.h"
#include "Catalyst/Utilities/GameTime.h"

namespace Catalyst::Network
{
	using Utilities::GameTime;

	NetworkTransform::NetworkTransform()
		: IPacketHandler(EGlobalPacketId::NetworkTransform), m_pos{ }, m_size{ }, m_rotation{ 0 }, m_origin{ },
		m_syncTime{ 0 }, m_syncInterval{ 1 }, m_targetId{ USHRT_MAX }
	{
	}

	bool NetworkTransform::Read(Packet* _packet)
	{
		m_pos.x = _packet->ReadFloat();
		m_pos.y = _packet->ReadFloat();

		m_size.x = _packet->ReadFloat();
		m_size.y = _packet->ReadFloat();

		m_rotation = _packet->ReadFloat();

		m_origin.x = _packet->ReadFloat();
		m_origin.y = _packet->ReadFloat();

		m_targetId = _packet->ReadShort();

		if (NetOwner()->Id()->NetId() == m_targetId)
			return true;

		_packet->ResetCursor();
		return false;
	}

	void NetworkTransform::Handle()
	{
		if (NetOwner()->Id()->NetId() != m_targetId)
			return;

		if (ActorTransform* transform = Owner()->Transform())
		{
			transform->position = m_pos;
			transform->size = m_size;
			transform->rotation = m_rotation;
			transform->origin = m_origin;
		}
	}

	void NetworkTransform::SetSyncInterval(const float _interval)
	{
		m_syncInterval = _interval;
	}

	void NetworkTransform::Tick()
	{
		m_syncTime += GameTime::DeltaTime();

		if(m_syncTime >= m_syncInterval)
		{
			m_syncTime = 0.f;

			if (const ActorTransform* transform = Owner()->Transform(); transform && HasAuthority())
			{
				Packet* transformPacket = new Packet(EGlobalPacketId::NetworkTransform);

				transformPacket->Write(transform->position.x);
				transformPacket->Write(transform->position.y);

				transformPacket->Write(transform->size.x);
				transformPacket->Write(transform->size.y);

				transformPacket->Write(transform->rotation);

				transformPacket->Write(transform->origin.x);
				transformPacket->Write(transform->origin.y);

				transformPacket->Write(NetOwner()->Id()->NetId());

				Network::Instance()->Send(transformPacket);
			}
		}
	}

	NetworkActor* NetworkTransform::NetOwner() const
	{
		return dynamic_cast<NetworkActor*>(Owner());
	}
}

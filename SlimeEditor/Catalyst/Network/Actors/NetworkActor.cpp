#include "NetworkActor.h"

#include "NetworkTransform.h"
#include "Catalyst/Network/NetworkId.h"
#include "Catalyst/Network/Network.h"

namespace Catalyst::Network
{
	NetworkActor::NetworkActor()
	{
		m_netTransform = CreateComponent<NetworkTransform>();
		m_id = CreateComponent<NetworkId>();
	}

	NetworkId* NetworkActor::Id() const
	{
		return m_id;
	}

	void NetworkActor::BeginPlay()
	{
		Actor::BeginPlay();

		Network::Instance()->Register(m_netTransform);
		Network::Instance()->Register(m_id);
	}

	void NetworkActor::EndPlay()
	{
		Actor::EndPlay();

		Network::Instance()->Deregister(m_id);
		Network::Instance()->Deregister(m_netTransform);
	}

	void NetworkActor::OnAuthorityStateChanged()
	{
		// sync the network transform's authority state
		if(HasAuthority())
		{
			m_netTransform->Grant();
			m_id->Grant();
		}
		else
		{
			m_netTransform->Revoke();
			m_id->Revoke();
		}
	}
}

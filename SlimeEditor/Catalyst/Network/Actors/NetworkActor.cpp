#include "NetworkActor.h"

#include "NetworkTransform.h"
#include "Catalyst/Network/NetId.h"
#include "Catalyst/Network/Network.h"

namespace Catalyst::Network
{
	NetworkActor::NetworkActor()
		: m_id{ new NetId(USHRT_MAX) }
	{
		m_netTransform = CreateComponent<NetworkTransform>();
	}

	void NetworkActor::BeginPlay()
	{
		Actor::BeginPlay();

		Network::Instance()->Register(m_netTransform);
	}

	void NetworkActor::EndPlay()
	{
		Actor::EndPlay();

		Network::Instance()->Deregister(m_netTransform);
	}

	void NetworkActor::OnAuthorityStateChanged()
	{
		// sync the network transform's authority state
		if(HasAuthority())
		{
			m_netTransform->Grant();
		}
		else
		{
			m_netTransform->Revoke();
		}
	}
}

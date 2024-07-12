#pragma once

#include "Catalyst/Actors/Actor.h"
#include "Catalyst/Network/IAuthorityProvider.h"

using Catalyst::Actors::Actor;

namespace Catalyst::Network
{
	class NetworkTransform;
	class NetId;

	class NetworkActor : public Actor, public IAuthorityProvider
	{
		friend class Network;

	public:
		NetworkActor();

	protected:
		void BeginPlay() override;

		void EndPlay() override;

		void OnAuthorityStateChanged() override;

	private:
		NetworkTransform* m_netTransform;
		NetId* m_id;

	};
}

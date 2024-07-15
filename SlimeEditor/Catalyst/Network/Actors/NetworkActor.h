#pragma once

#include "Catalyst/Actors/Actor.h"
#include "Catalyst/Network/IAuthorityProvider.h"

using Catalyst::Actors::Actor;

namespace Catalyst::Network
{
	class NetworkTransform;
	class NetworkId;

	class NetworkActor : public Actor, public IAuthorityProvider
	{
		friend class Network;

	public:
		NetworkActor();

	public:
		NetworkId* Id() const;

	protected:
		void BeginPlay() override;

		void EndPlay() override;

		void OnAuthorityStateChanged() override;

	protected:
		NetworkTransform* m_netTransform;
		NetworkId* m_id;

	};
}

#include "IAuthorityProvider.h"

namespace Catalyst::Network
{
	IAuthorityProvider::IAuthorityProvider()
		: m_hasAuthority{ false }
	{
	}

	void IAuthorityProvider::Grant()
	{
		m_hasAuthority = true;

		OnAuthorityStateChanged();
	}

	void IAuthorityProvider::Revoke()
	{
		m_hasAuthority = false;

		OnAuthorityStateChanged();
	}

	bool IAuthorityProvider::HasAuthority() const
	{
		return m_hasAuthority;
	}

	void IAuthorityProvider::OnAuthorityStateChanged()
	{
	}
}

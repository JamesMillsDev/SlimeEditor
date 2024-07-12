#pragma once

namespace Catalyst::Network
{
	class IAuthorityProvider
	{
	public:
		IAuthorityProvider();

	public:
		void Grant();
		void Revoke();

		[[nodiscard]] bool HasAuthority() const;

	protected:
		virtual void OnAuthorityStateChanged();

	private:
		bool m_hasAuthority;

	};
}
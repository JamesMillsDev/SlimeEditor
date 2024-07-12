#pragma once
#include "Catalyst/Types.h"

namespace Catalyst::Network
{
	class NetId
	{
	public:
		NetId() = delete;
		NetId(byte _id);

	public:
		[[nodiscard]] byte Id() const;

	public:
		operator byte() const;

	private:
		byte m_id;

	};
}

#include "INetworkSide.h"

namespace Catalyst::Network
{
	INetworkSide::INetworkSide(const size_t _channels, const enet_uint32 _bandwidthOut, const enet_uint32 _bandwidthIn)
		: m_host{ nullptr }, m_address{ 0, 0 }, m_channels{ _channels }, m_bandwidthOut{ _bandwidthOut },
		m_bandwidthIn{ _bandwidthIn }
	{
	}
}

#pragma once

#include <enet/enet.h>

namespace Catalyst::Network
{
	class INetworkSide
	{
	public:
		INetworkSide(size_t _channels, enet_uint32 _bandwidthOut, enet_uint32 _bandwidthIn);
		virtual ~INetworkSide() = default;

		INetworkSide(const INetworkSide&) = delete;
		INetworkSide(INetworkSide&&) = delete;

	public:
		virtual bool Start() = 0;
		virtual void Tick() = 0;
		virtual void Stop() const = 0;
		virtual bool IsServer() = 0;

		[[nodiscard]] ENetHost* Host() const { return m_host; }
		virtual bool Ready() { return m_host != nullptr; }

	public:
		INetworkSide& operator=(const INetworkSide&) = delete;
		INetworkSide& operator=(INetworkSide&&) = delete;

	protected:
		ENetHost* m_host;
		ENetAddress m_address;

		size_t m_channels;
		enet_uint32 m_bandwidthOut;
		enet_uint32 m_bandwidthIn;

	};
}

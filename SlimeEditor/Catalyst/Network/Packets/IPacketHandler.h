#pragma once
#include <enet/types.h>

namespace Catalyst::Network
{
	class Packet;

	class IPacketHandler
	{
	public:
		IPacketHandler() = delete;
		explicit IPacketHandler(enet_uint8 _id);

		virtual ~IPacketHandler() = default;

		IPacketHandler(const IPacketHandler&) = delete;
		IPacketHandler(IPacketHandler&&) = delete;

	public:
		[[nodiscard]] enet_uint8 Id() const;

		virtual void Read(Packet* _packet) = 0;
		virtual void Handle() = 0;

	public:
		IPacketHandler& operator=(const IPacketHandler&) = delete;
		IPacketHandler& operator=(IPacketHandler&&) = delete;

	private:
		enet_uint8 m_id;

	};
}

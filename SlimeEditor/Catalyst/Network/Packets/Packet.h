#pragma once

#include <stdexcept>
#include <enet/enet.h>

namespace Catalyst::Network
{
	class INetworkSide;

	class Packet
	{
		friend class Network;

	public:
		explicit Packet(enet_uint8 _id);
		explicit Packet(ENetPacket* _packet);

	public:
		float ReadFloat();
		int ReadInt();
		bool ReadBool();
		enet_uint8 ReadByte();
		char* ReadString();

		void Write(float _float);
		void Write(enet_uint8 _byte);
		void Write(int _int);
		void Write(bool _bool);
		void Write(char* _string, size_t _len);

		[[nodiscard]] enet_uint8 Id() const;

	protected:
		void WriteBytes(const enet_uint8* _bytes, size_t _size);

		template<typename T>
		T ReadBytes();

	private:
		ENetPacket* m_packet;
		size_t m_cursor;
		enet_uint8 m_id;

	private:
		void Broadcast(const INetworkSide* _network, enet_uint8 _channel) const;

	};

	template <typename T>
	T Packet::ReadBytes()
	{
		if (m_cursor + sizeof(T) > m_packet->dataLength)
			throw std::overflow_error("Buffer does not contain any remaining strings.");

		T data;
		memcpy(&data, &m_packet->data[m_cursor], sizeof(T));

		m_cursor += sizeof(T);

		return data;
	}
}

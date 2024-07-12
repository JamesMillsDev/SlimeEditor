#include "Packet.h"

#include <cstring>
#include <stdexcept>

#include "Catalyst/Network/INetworkSide.h"

namespace Catalyst::Network
{
	Packet::Packet(const enet_uint8 _id)
		: m_packet{ nullptr }, m_cursor{ 1 }, m_id{ _id }
	{
		m_packet = enet_packet_create(new unsigned char(m_id), sizeof(enet_uint8), ENET_PACKET_FLAG_RELIABLE);
	}

	Packet::Packet(ENetPacket* _packet)
		: m_packet{ _packet }, m_cursor{ 0 }
	{
		// Read the first byte as the packet id
		m_id = ReadByte();
	}

	float Packet::ReadFloat()
	{
		return ReadBytes<float>();
	}

	int Packet::ReadInt()
	{
		return ReadBytes<int>();
	}

	bool Packet::ReadBool()
	{
		return ReadBytes<bool>();
	}

	enet_uint8 Packet::ReadByte()
	{
		return ReadBytes<enet_uint8>();
	}

	char* Packet::ReadString()
	{
		const size_t stringLen = ReadBytes<size_t>();

		// Special handling of string required
		if (m_cursor + stringLen * sizeof(char) > m_packet->dataLength)
			throw std::overflow_error("Buffer does not contain any remaining strings.");

		char* string = new char[stringLen + 1];
		memcpy(string, &m_packet->data[m_cursor], stringLen * sizeof(char));

		string[stringLen] = '\0';
		m_cursor += stringLen * sizeof(char);

		return string;
	}

	void Packet::Write(float _float)
	{
		WriteBytes(reinterpret_cast<enet_uint8*>(&_float), sizeof(float));
	}

	void Packet::Write(const enet_uint8 _byte)
	{
		WriteBytes(&_byte, sizeof(enet_uint8));
	}

	void Packet::Write(int _int)
	{
		WriteBytes(reinterpret_cast<enet_uint8*>(&_int), sizeof(int));
	}

	void Packet::Write(bool _bool)
	{
		WriteBytes(reinterpret_cast<enet_uint8*>(&_bool), sizeof(bool));
	}

	void Packet::Write(char* _string, size_t _len)
	{
		WriteBytes(reinterpret_cast<enet_uint8*>(&_len), sizeof(size_t));
		WriteBytes(reinterpret_cast<enet_uint8*>(_string), sizeof(char) * _len);
	}

	enet_uint8 Packet::Id() const
	{
		return m_id;
	}

	void Packet::WriteBytes(const enet_uint8* _bytes, const size_t _size)
	{
		// Resize the packet to the current size + the incoming size
		enet_packet_resize(m_packet, m_packet->dataLength + _size);

		// write the bytes passed into the data buffer
		for(size_t i = 0; i < _size; ++i)
			m_packet->data[m_cursor + i] = _bytes[i];

		// move the cursor position to the new end
		m_cursor += _size;
	}

	void Packet::Broadcast(const INetworkSide* _network, const enet_uint8 _channel) const
	{
		enet_host_broadcast(_network->Host(), _channel, m_packet);
	}
}

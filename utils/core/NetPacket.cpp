#include "NetPacket.h"
#include <cstring>

NetPacket::NetPacket()
{
	memset(m_pBuff,0,sizeof(m_pBuff));
	m_startPos = 0;
	m_length = 0;
}

NetPacket::~NetPacket()
{
}

// void NetPacket::setBuffer(char* p, int size)
// {
// 	if (m_pBuff)
// 	{
// 		delete m_pBuff;
// 	}
// 
// 	if (size <= 1024)
// 	{
// 		if (size <= 32)
// 		{
// 			m_pBuff = new unsigned char[32];
// 		}
// 		else if (size <= 64)
// 		{
// 			m_pBuff = new unsigned char[64];
// 		}
// 		else if (size <= 128)
// 		{
// 			m_pBuff = new unsigned char[128];
// 		}
// 		else if (size <= 256)
// 		{
// 			m_pBuff = new unsigned char[256];
// 		}
// 		else if (size <= 512)
// 		{
// 			m_pBuff = new unsigned char[512];
// 		}
// 		else
// 		{
// 			m_pBuff = new unsigned char[1024];
// 		}
// 	}
// 	else if (size <= 4096)
// 	{
// 		m_pBuff = new unsigned char[4096];
// 	}
// 	else if (size <= 8192)
// 	{
// 		m_pBuff = new unsigned char[8192];
// 	}
// 	else if (size <= 32768)
// 	{
// 		m_pBuff = new unsigned char[32768];
// 	}
// 	else
// 	{
// 		m_pBuff = new unsigned char[65536];
// 	}
// 
// 	memcpy(m_pBuff,p,size);
// }

void NetPacket::setHeaderInfo(uint16 op_code, uint16 packet_size)
{
	m_header.opcode = op_code;
	m_header.packetSize = packet_size;
}

void NetPacket::setData(unsigned char* p, int length)
{
	memcpy(m_pBuff,p,length);
	m_startPos = 0;
	m_length = length;
}

void NetPacket::decode(mdk::NetHost host)
{
	host.Recv(reinterpret_cast<uint8*>(&m_header), PACKET_HEADER_SIZE, true);
}

int8 NetPacket::readInt8()
{
	int8 value;
	if (m_startPos + sizeof(value) > m_length)
	{
		throw std::string("NetPacket::readInt8 exception");
	}
	memcpy(&value, m_pBuff + m_startPos, sizeof(value));
	m_startPos += sizeof(value);
	return value;
}

uint8 NetPacket::readUint8()
{
	uint8 value;
	if (m_startPos + sizeof(value) > m_length)
	{
		throw std::string("NetPacket::readUint8 exception");
	}
	memcpy(&value, m_pBuff + m_startPos, sizeof(value));
	m_startPos += sizeof(value);
	return value;
}

bool NetPacket::readBool()
{
	bool value;
	if (m_startPos + sizeof(value) > m_length)
	{
		throw std::string("NetPacket::readBool exception");
	}
	memcpy(&value, m_pBuff + m_startPos, sizeof(value));
	m_startPos += sizeof(value);
	return value;
}

int16 NetPacket::readInt16()
{
	int16 value;
	if (m_startPos + sizeof(value) > m_length)
	{
		throw std::string("NetPacket::readInt16 exception");
	}
	memcpy(&value, m_pBuff + m_startPos, sizeof(value));
	m_startPos += sizeof(value);
	value = _BITSWAP16(value);
	return value;
}

uint16 NetPacket::readUint16()
{
	uint16 value;
	if (m_startPos + sizeof(value) > m_length)
	{
		throw std::string("NetPacket::readUint16 exception");
	}
	memcpy(&value, m_pBuff + m_startPos, sizeof(value));
	m_startPos += sizeof(value);
	value = _BITSWAP16(value);
	return value;
}

int NetPacket::readInt()
{
	int value;
	if (m_startPos + sizeof(value) > m_length)
	{
		throw std::string("NetPacket::readInt exception");
	}
	memcpy(&value, m_pBuff + m_startPos, sizeof(value));
	m_startPos += sizeof(value);
	value = _BITSWAP32(value);
	return value;
}

uint32 NetPacket::readUint32()
{
	uint32 value;
	if (m_startPos + sizeof(value) > m_length)
	{
		throw std::string("NetPacket::readUint32 exception");
	}
	memcpy(&value, m_pBuff + m_startPos, sizeof(value));
	m_startPos += sizeof(value);
	value = _BITSWAP32(value);
	return value;
}

std::string NetPacket::readString()
{
	uint16 size;
	if (m_startPos + sizeof(size) > m_length)
	{
		throw std::string("NetPacket::readString read string length exception");
	}
	memcpy(&size, m_pBuff + m_startPos, sizeof(size));
	size = _BITSWAP16(size);
	m_startPos += sizeof(size);
	if (m_startPos + size > m_length)
	{
		throw std::string("NetPacket::readString read string content exception");
	}
	uint8* tmpBuf = new uint8[size];
	memcpy(tmpBuf, m_pBuff + m_startPos, size);
	m_startPos += size;
	std::string str;
	str.append((char*)tmpBuf,size);
	delete [] tmpBuf;
	return str;
}

int64 NetPacket::readInt64()
{
	int64 value;
	if (m_startPos + sizeof(value) > m_length)
	{
		throw std::string("NetPacket::readInt64 exception");
	}
	memcpy(&value, m_pBuff + m_startPos, sizeof(value));
	m_startPos += sizeof(value);
	return _BITSWAP64(value);
}

void NetPacket::encode()
{

}

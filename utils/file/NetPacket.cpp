#include "NetPacket.h"
#include <cstring>

NetPacket::NetPacket()
{

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

void NetPacket::decode(mdk::NetHost host)
{
	host.Recv(reinterpret_cast<uint8*>(&m_header), PACKET_HEADER_SIZE, true);
}

int8 NetPacket::readInt8(mdk::NetHost& host)
{
	int8 value;
	if (!host.Recv(reinterpret_cast<uint8*>(&value), sizeof(int8), true))
	{
		throw std::string("NetPacket::readInt8 exception");
	}
	return value;
}

uint8 NetPacket::readUint8(mdk::NetHost& host)
{
	uint8 value;
	if (!host.Recv(reinterpret_cast<uint8*>(&value), sizeof(uint8), true))
	{
		throw std::string("NetPacket::readUint8 exception");
	}
	return value;
}

bool NetPacket::readBool(mdk::NetHost& host)
{
	bool value;
	if (!host.Recv(reinterpret_cast<uint8*>(&value), sizeof(bool), true))
	{
		throw std::string("NetPacket::readBool exception");
	}
	return value;
}

int16 NetPacket::readInt16(mdk::NetHost& host)
{
	int16 value;
	if (!host.Recv(reinterpret_cast<uint8*>(&value), sizeof(int16), true))
	{
		throw std::string("NetPacket::readInt16 exception");
	}
	value = _BITSWAP16(value);
	return value;
}

uint16 NetPacket::readUint16(mdk::NetHost& host)
{
	uint16 value;
	if (!host.Recv(reinterpret_cast<uint8*>(&value), sizeof(uint16), true))
	{
		throw std::string("NetPacket::readUint16 exception");
	}
	value = _BITSWAP16(value);
	return value;
}

int NetPacket::readInt(mdk::NetHost& host)
{
	int value;
	if (!host.Recv(reinterpret_cast<uint8*>(&value), sizeof(int), true))
	{
		throw std::string("NetPacket::readInt exception");
	}
	value = _BITSWAP32(value);
	return value;
}

uint32 NetPacket::readUint32(mdk::NetHost& host)
{
	uint32 value;
	if (!host.Recv(reinterpret_cast<uint8*>(&value), sizeof(uint32), true))
	{
		throw std::string("NetPacket::readUint32 exception");
	}
	value = _BITSWAP32(value);
	return value;
}

std::string NetPacket::readString(mdk::NetHost& host)
{
	uint16 size;
	if (!host.Recv(reinterpret_cast<uint8*>(&size), sizeof(uint16), true))
	{
		throw std::string("NetPacket::readString size exception");
	}
	size = _BITSWAP16(size);
	uint8* tmpBuf = new uint8[size];
	if (!host.Recv(reinterpret_cast<uint8*>(tmpBuf), size, true))
	{
		delete [] tmpBuf;
		throw std::string("NetPacket::readString content exception");
	}
	std::string str;
	str.append((char*)tmpBuf,size);
	delete [] tmpBuf;
	return str;
}

int64 NetPacket::readInt64(mdk::NetHost& host)
{
	int64 value;
	if (!host.Recv(reinterpret_cast<uint8*>(&value), sizeof(int64), true))
	{
		throw std::string("NetPacket::readInt64 exception");
	}
	return _BITSWAP64(value);
}

void NetPacket::encode()
{

}

#pragma once

#include "TypeDef.h"
#include "NetHost.h"

#pragma pack(push, 4)

struct NetPacketHeader
{
	uint16 packetSize;
	uint16 opcode;
};

#define PACKET_HEADER_SIZE sizeof(NetPacketHeader)

#pragma pack(pop)

class NetPacket
{
public:
	NetPacket();
	~NetPacket();

public:
	//void setBuffer(char* p, int size);
	virtual void decode(mdk::NetHost host);
	virtual void encode();

protected:
	int8 readInt8(mdk::NetHost& host);
	uint8 readUint8(mdk::NetHost& host);
	bool readBool(mdk::NetHost& host);
	int16 readInt16(mdk::NetHost& host);
	uint16 readUint16(mdk::NetHost& host);
	int readInt(mdk::NetHost& host);
	uint32 readUint32(mdk::NetHost& host);
	std::string readString(mdk::NetHost& host);
	int64 readInt64(mdk::NetHost& host);
private:
	NetPacketHeader m_header;
// 	unsigned char* m_pBuff;
// 	bool m_bParseHeader;
// 	int m_startPos;
// 	int m_writePos;
};
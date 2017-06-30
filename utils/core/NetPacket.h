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
	void setHeaderInfo(uint16 op_code, uint16 packet_size);
	void setData(unsigned char* p, int length);
public:
	int8 readInt8();
	uint8 readUint8();
	bool readBool();
	int16 readInt16();
	uint16 readUint16();
	int readInt();
	uint32 readUint32();
	std::string readString();
	int64 readInt64();
private:
	NetPacketHeader m_header;
	unsigned char m_pBuff[10 * 1024];
// 	unsigned char* m_pBuff;
// 	bool m_bParseHeader;
 	int m_startPos;
	int m_length;
// 	int m_writePos;
};
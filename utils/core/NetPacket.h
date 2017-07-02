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
	void setHeaderInfo(uint16 op_code, uint16 packet_size);
	void setData(unsigned char* p, int length);
	uint16 getOpcode(){ return m_header.opcode; };
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

	void writeInt8(int8 value);
	void writeUint8(uint8 value);
	void writeBool(bool value);
	void writeInt16(int16 value);
	void writeUint16(uint16 value);
	void writeInt(int value);
	void writeUint32(uint32 value);
	void writeString(std::string value);
	void writeInt64(int64 value);
private:
	NetPacketHeader m_header;
	unsigned char m_pBuff[10 * 1024];
// 	unsigned char* m_pBuff;
// 	bool m_bParseHeader;
 	int m_startPos;
	int m_length;
// 	int m_writePos;
};
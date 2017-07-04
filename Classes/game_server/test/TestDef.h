#pragma once

#include "core/NetPacket.h"
#include "core/FileUtil.h"
#include "core/Cast.h"

struct TestMsg
{
	void decode(NetPacket& packet)
	{
		number = packet.readInt();
		str = packet.readString();
		s = packet.readInt16();
		LOG(std::format("number = %d str = %s s = %d", number, str.c_str(), s));
	}

	void encode(NetPacket& packet)
	{
		packet.writeInt(number);
		packet.writeString(str);
		packet.writeInt16(s);
	}

	int number;
	std::string str;
	short s;
};
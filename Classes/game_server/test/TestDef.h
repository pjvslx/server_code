#pragma once

#include "file/NetPacket.h"
#include "file/FileUtil.h"
#include "file/Cast.h"

struct TestMsg : NetPacket
{
	void encode()
	{
		
	}
	void decode(mdk::NetHost host)
	{
		NetPacket::decode(host);
		number = readInt(host);
		str = readString(host);
		s = readInt16(host);
		LOG(std::format("number = %d str = %s s = %d", number, str.c_str(), s));
	}

	int number;
	std::string str;
	short s;
};
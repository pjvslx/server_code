#pragma once

#include "core/TypeDef.h"
#include <string>
#include "core/NetPacket.h"

struct stPlayerCharacterInfo : NetPacket
{
	uint32 AccountID;       //账号id
	uint32 GoldCoins;       //金币

	void decode(mdk::NetHost host)
	{
		NetPacket::decode(host);
		AccountID = readUint32(host);
		GoldCoins = readUint32(host);
	}
};

struct stPlayerAccountInfo : NetPacket	// 账号信息
{
	uint32 accountID;       // 账号id
	std::string account;    // 用户绑定账号
	std::string password;   // 用户绑定账号密码
	std::string nick;       // 昵称
	std::string signature;  // 签名
	uint32 icon;            // (暂时)玩家头像编号
	uint32 sex;             // 性别   //ePLAYER_SEX
	uint32 birthday;        // 生日，时间戳
	std::string thirdlogin; // 第三方平台绑定状态
	uint32 cr_date;         // 账号创建时间
	std::string avatar_url; // 头像

	void decode(mdk::NetHost host)
	{
		NetPacket::decode(host);
		accountID = readUint32(host);
		account = readString(host);
		password = readString(host);
		nick = readString(host);
		signature = readString(host);
		icon = readUint32(host);
		sex = readUint32(host);
		birthday = readUint32(host);
		thirdlogin = readString(host);
		cr_date = readUint32(host);
		avatar_url = readString(host);
	}
};

struct stPlayerAccountCharacterInfo
{
	stPlayerAccountInfo account;
	stPlayerCharacterInfo character;
};
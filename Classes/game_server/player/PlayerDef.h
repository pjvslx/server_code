#pragma once

#include "core/TypeDef.h"
#include <string>
#include "core/NetPacket.h"
#include "core/FileUtil.h"

struct stPlayerCharacterInfo
{
	uint32 AccountID;       //账号id
	uint32 GoldCoins;       //金币

	void decode(NetPacket& packet)
	{
		AccountID = packet.readUint32();
		GoldCoins = packet.readUint32();
	}
};

struct stPlayerAccountInfo
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

	void decode(NetPacket& packet)
	{
		accountID = packet.readUint32();
		account = packet.readString();
		password = packet.readString();
		nick = packet.readString();
		signature = packet.readString();
		icon = packet.readUint32();
		sex = packet.readUint32();
		birthday = packet.readUint32();
		thirdlogin = packet.readString();
		cr_date = packet.readUint32();
		avatar_url = packet.readString();
	}
};

struct stPlayerAccountCharacterInfo
{
	stPlayerAccountInfo account;
	stPlayerCharacterInfo character;
	void decode(NetPacket& packet)
	{
		account.decode(packet);
		character.decode(packet);
	}
};

struct C2GMG2GL_AccountLogin
{
	uint32 loginType;
	std::string version;
	uint32 loginTime;
	/*
	IMEI	机器码快速登陆模式	机器码,"XXX'de phone",""
	ACCOUNT 用户名密码登陆模式	用户名,密码,""
	THIRD	第三方快速登陆模式	第三方uid,gameToken,第三方来源
	*/
	std::string userName;
	std::string userStr;
	std::string provider;

	void decode(NetPacket& packet)
	{
		loginType = packet.readUint32();
		loginTime = packet.readUint32();
		userName = packet.readString();
		userStr = packet.readString();
		provider = packet.readString();
		version = packet.readString();
		LOG(std::format("loginType = %u loginTime = %u userName = %s userStr = %s provider = %s version = %s", \
			loginType, loginTime, userName.c_str(), userStr.c_str(), provider.c_str(), version.c_str()));
	}

	//loginType << loginTime << userName << userStr << provider << version << clientId1 << clientId2;
};
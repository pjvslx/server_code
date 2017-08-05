#pragma once
#include <map>
#include "core/TypeDef.h"
#include <string>
#include "core/NetPacket.h"
#include "core/FileUtil.h"
#include "prop/PropDef.h"

typedef int32 stPropId;
typedef std::map<stPropId, uint32> stPlayerPropMap;

struct stPlayerPropTable //玩家道具列表
{
	uint32 AccountID;   //账号id
	stPlayerPropMap m_propMap;

	stPlayerPropTable() : AccountID(0) {

	}

	uint32 getPropCount(int ID){
		stPlayerPropMap::iterator it = m_propMap.find(ID);
		if (it == m_propMap.end())
		{
			return 0;
		}
		return it->second;
	}


	uint32 getPropValue(stPropId propKey) const
	{
		stPlayerPropMap::const_iterator it = m_propMap.find(propKey);
		if (it != m_propMap.end()) {
			return it->second;
		}
		else {
			return 0;
		}
	}

	void setPropValue(stPropId propKey, uint32 value)
	{
		m_propMap[propKey] = value;
	}

	void encode(NetPacket& packet)
	{
		packet.writeUint32(AccountID);
		packet.writeUint32(m_propMap.size());
		for (stPlayerPropMap::iterator it = m_propMap.begin(); it != m_propMap.end(); it++)
		{
			packet.writeInt(it->first);
			packet.writeUint32(it->second);
		}
	}
};

struct stPlayerCharacterInfo
{
	uint32 AccountID;       //账号id
	uint32 GoldCoins;       //金币

	stPlayerCharacterInfo()
		: AccountID(0)
		, GoldCoins(0)
	{

	}

	void decode(NetPacket& packet)
	{
		AccountID = packet.readUint32();
		GoldCoins = packet.readUint32();
	}

	void encode(NetPacket& packet)
	{
		packet.writeUint32(AccountID);
		packet.writeUint32(GoldCoins);
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

	stPlayerAccountInfo()
		: accountID(0)
		, icon(0)
		, sex(0)
		, birthday(0)
		, cr_date(0)
	{

	}

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

	void encode(NetPacket& packet)
	{
		packet.writeUint32(accountID);
		packet.writeString(account);
		packet.writeString(password);
		packet.writeString(nick);
		packet.writeString(signature);
		packet.writeUint32(icon);
		packet.writeUint32(sex);
		packet.writeUint32(birthday);
		packet.writeString(thirdlogin);
		packet.writeUint32(cr_date);
		packet.writeString(avatar_url);
	}
};

struct stPlayerAccountCharacterInfo
{
	stPlayerAccountInfo account;
	stPlayerCharacterInfo character;
	stPlayerPropTable propTable;

	void encode(NetPacket& packet)
	{
		account.encode(packet);
		character.encode(packet);
		propTable.encode(packet);
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
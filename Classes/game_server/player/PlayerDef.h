#pragma once
#include <map>
#include "core/TypeDef.h"
#include <string>
#include "core/NetPacket.h"
#include "core/FileUtil.h"
#include "prop/PropDef.h"

typedef int32 stPropId;
typedef std::map<stPropId, uint32> stPlayerPropMap;

struct stPlayerPropTable //��ҵ����б�
{
	uint32 AccountID;   //�˺�id
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
	uint32 AccountID;       //�˺�id
	uint32 GoldCoins;       //���

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
	uint32 accountID;       // �˺�id
	std::string account;    // �û����˺�
	std::string password;   // �û����˺�����
	std::string nick;       // �ǳ�
	std::string signature;  // ǩ��
	uint32 icon;            // (��ʱ)���ͷ����
	uint32 sex;             // �Ա�   //ePLAYER_SEX
	uint32 birthday;        // ���գ�ʱ���
	std::string thirdlogin; // ������ƽ̨��״̬
	uint32 cr_date;         // �˺Ŵ���ʱ��
	std::string avatar_url; // ͷ��

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
	IMEI	��������ٵ�½ģʽ	������,"XXX'de phone",""
	ACCOUNT �û��������½ģʽ	�û���,����,""
	THIRD	���������ٵ�½ģʽ	������uid,gameToken,��������Դ
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
#pragma once

#include "core/TypeDef.h"
#include <string>
#include "core/NetPacket.h"
#include "core/FileUtil.h"

struct stPlayerCharacterInfo
{
	uint32 AccountID;       //�˺�id
	uint32 GoldCoins;       //���

	void decode(NetPacket& packet)
	{
		AccountID = packet.readUint32();
		GoldCoins = packet.readUint32();
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
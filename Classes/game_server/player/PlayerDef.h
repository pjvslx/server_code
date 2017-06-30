#pragma once

#include "core/TypeDef.h"
#include <string>
#include "core/NetPacket.h"

struct stPlayerCharacterInfo : NetPacket
{
	uint32 AccountID;       //�˺�id
	uint32 GoldCoins;       //���

	void decode(mdk::NetHost host)
	{
		NetPacket::decode(host);
		AccountID = readUint32(host);
		GoldCoins = readUint32(host);
	}
};

struct stPlayerAccountInfo : NetPacket	// �˺���Ϣ
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
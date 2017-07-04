#include <functional>
#include "PlayerManager.h"
#include "MsgDispatcher.h"
#include "OpCode.h"
#include "PlayerDef.h"
#include "GameServer.h"
#include "test/TestDef.h"
using namespace std;

PlayerManager* PlayerManager::m_instance = nullptr;

PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{

}

bool PlayerManager::init()
{
	MsgDispatcher::getInstance()->addPacketObserver(typeid(this).name(),
		bind(&PlayerManager::__handlePacket, this, placeholders::_1, placeholders::_2)
		, CMSG_ACCOUNT_LOGIN
		, NULL);
	return true;
}

PlayerManager* PlayerManager::getInstance()
{
	if (!m_instance)
	{
		m_instance = new PlayerManager();
	}

	return m_instance;
}

void PlayerManager::__handlePacket(unsigned int opcode, void* resp)
{
	NetPacket *recvPackage = static_cast<NetPacket*>(resp);
	switch (opcode)
	{
	case CMSG_ACCOUNT_LOGIN:
		__handleClientLogin(resp);
		break;
	default:
		break;
	}
}

void PlayerManager::__handleClientLogin(void* resp)
{
	NetPacket* p = static_cast<NetPacket*>(resp);
	C2GMG2GL_AccountLogin accountInfo;
	accountInfo.decode(*p);
	map<int64, Player*>::iterator it = m_mapIdPlayer.find(p->getConnectId());
	if (it == m_mapIdPlayer.end())
	{
		addPlayer(p->getConnectId());
	}

	NetPacket pack(100);
	TestMsg msg;
	msg.number = 1;
	msg.str = "fuck way";
	msg.s = 5;
	msg.encode(pack);
	sendPacket(p->getConnectId(), &pack);
}

void PlayerManager::addPlayer(int64 connect_id)
{
	Player* pPlayer = new Player;
	stPlayerAccountCharacterInfo info;
	m_playerSeqId++;
	info.account.accountID = m_playerSeqId;
	info.character.AccountID = m_playerSeqId;
	m_mapIdPlayer[connect_id] = pPlayer;
	m_mapAcountIdPlayer[m_playerSeqId] = pPlayer;
}

void PlayerManager::removePlayer(int64 connect_id)
{
	map<int64, Player*>::iterator it = m_mapIdPlayer.find(connect_id);
	if (it != m_mapIdPlayer.end())
	{
		uint32 accountId = it->second->getPlayerInfo().account.accountID;
		delete it->second;
		m_mapIdPlayer.erase(it);

		map<uint32, Player*>::iterator it = m_mapAcountIdPlayer.find(accountId);
		if (it != m_mapAcountIdPlayer.end())
		{
			m_mapAcountIdPlayer.erase(accountId);
		}
	}
}

char* PlayerManager::__encodePacket(NetPacket* p)
{
	assert(p);
	char* buff = new char[10*1024];
	uint16 size = _BITSWAP16(p->getPacketContentSize() + PACKET_HEADER_SIZE);
	uint16 opcode = _BITSWAP16(p->getOpcode());
	int startPos = 0;
	char tmp[10 * 1024] = {0};
	memcpy(buff, (int8*)&size, sizeof(size));
	memcpy(&tmp[startPos], &size, sizeof(size));
	startPos += sizeof(size);
	memcpy(buff + startPos, (int8*)&opcode, sizeof(opcode));
	startPos += sizeof(opcode);
	memcpy(buff + startPos, p->getData(), size);
	return buff;
}

void PlayerManager::sendPacket(int64 connectId, NetPacket* p)
{
	assert(p);
	GameServer* gs = GameServer::getInstance();
	char* buff = __encodePacket(p);

	uint16 size;
	uint16 opcode;
	int x;
	short s;
	memcpy(&size,buff,sizeof(size));
	memcpy(&opcode, buff + 2, sizeof(opcode));
	memcpy(&x, buff + 4, sizeof(x));
	memcpy(&s, buff + 8, sizeof(s));
	gs->SendMsg(connectId, buff, PACKET_HEADER_SIZE + p->getPacketContentSize());
	delete[] buff;
}

void PlayerManager::sendPacket(uint32 playerId, NetPacket* p)
{
	assert(p);
	GameServer* gs = GameServer::getInstance();
	char* buff = __encodePacket(p);
	Player* pPlayer = m_mapAcountIdPlayer[playerId];
	if (pPlayer)
	{
		gs->SendMsg(pPlayer->getConnectId(), buff, PACKET_HEADER_SIZE + p->getPacketContentSize());
	}

	delete[] buff;
}

void PlayerManager::sendPacket(const std::vector<uint32>& vecPlayerId, NetPacket* p)
{
	assert(p);
	GameServer* gs = GameServer::getInstance();
	char* buff = __encodePacket(p);
	for (int i = 0; i < vecPlayerId.size(); i++)
	{
		Player* pPlayer = m_mapAcountIdPlayer[vecPlayerId[i]];
		if (pPlayer)
		{
			gs->SendMsg(pPlayer->getConnectId(), buff, PACKET_HEADER_SIZE + p->getPacketContentSize());
		}
	}

	delete[] buff;
}
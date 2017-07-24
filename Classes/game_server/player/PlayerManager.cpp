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
		bind(&PlayerManager::__handlePacket, this, placeholders::_1, placeholders::_2, placeholders::_3)
		, CMSG_ACCOUNT_LOGIN
		, CMSG_PLAYER_INFO
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

void PlayerManager::__handlePacket(int64 connectId,unsigned int opcode, void* resp)
{
	NetPacket *recvPackage = static_cast<NetPacket*>(resp);
	switch (opcode)
	{
	case CMSG_ACCOUNT_LOGIN:
		__handleClientLogin(connectId, resp);
	case SMSG_KEEP_ACTIVE:
		__handleKeepAlive(connectId, resp);
	case CMSG_PLAYER_INFO:
		__handlePlayerInfo(connectId, resp);
		break;
	default:
		break;
	}
}

void PlayerManager::__handlePlayerInfo(int64 connectId, void* resp)
{
	Player* player = m_mapIdPlayer[connectId];
	if (!player)
	{
		__kickPlayer(connectId);
		return;
	}

	NetPacket pack(SMSG_PLAYER_INFO);
	stPlayerAccountCharacterInfo info = player->getPlayerInfo();
	info.encode(pack);
	sendPacket(connectId, &pack);
}

void PlayerManager::__handleKeepAlive(int64 connectId, void* resp)
{

}

void PlayerManager::__handleClientLogin(int64 connectId, void* resp)
{
	NetPacket* p = static_cast<NetPacket*>(resp);
	C2GMG2GL_AccountLogin accountInfo;
	accountInfo.decode(*p);
	map<std::string, Player*>::iterator it = m_mapUserIdPlayer.find(accountInfo.userStr);
	Player* player = nullptr;
	stPlayerAccountCharacterInfo info;
	if (it != m_mapUserIdPlayer.end())
	{
		if (it->second->getConnectId() == connectId)
		{
			//同一个connection发送了两次登陆 忽略
			return;
		}
		//关掉原先的connection以及释放原先的用户
		__kickPlayer(it->second->getConnectId());
		player = it->second;
		player->reset();
		m_mapIdPlayer.erase(connectId);
		info = player->getPlayerInfo();
	}
	else
	{
		player = new Player;
		m_playerSeqId++;
		info.account.accountID = m_playerSeqId;
		info.character.AccountID = m_playerSeqId;
		info.account.nick = accountInfo.userName;
		info.propTable.setPropValue(kPropGoldCoins,1000);
		info.propTable.setPropValue(kPropHouseCard,5);
		player->setPlayerInfo(info);
	}

	player->setConnectId(connectId);
	m_mapIdPlayer.insert(make_pair(connectId, player));

	NetPacket pack(SMSG_LOGIN_RESP);
	sendPacket(connectId,&pack);
}

void PlayerManager::__kickPlayer(int64 connectId)
{
	GameServer::getInstance()->CloseConnect(connectId);
}

void* PlayerManager::removePlayerByConnection(void*obj, void* p)
{
	int64 connect_id = *(int64*)p;
	removePlayer(connect_id);
	return nullptr;
}

void PlayerManager::removePlayer(int64 connect_id)
{
	map<int64, Player*>::iterator it = m_mapIdPlayer.find(connect_id);
	if (it != m_mapIdPlayer.end())
	{
		uint32 accountId = it->second->getPlayerInfo().account.accountID;
		string userId = it->second->getPlayerInfo().account.account;
		delete it->second;
		m_mapIdPlayer.erase(it);
		m_mapAcountIdPlayer.erase(accountId);
		m_mapUserIdPlayer.erase(userId);
	}
}

void PlayerManager::removePlayer(std::string userId)
{
	MapUserPlayer::iterator it = m_mapUserIdPlayer.find(userId);
	if (it != m_mapUserIdPlayer.end())
	{
		uint32 account_id = it->second->getPlayerId();
		int64 connect_id = it->second->getConnectId();
		delete it->second;
		m_mapUserIdPlayer.erase(it);
		m_mapAcountIdPlayer.erase(account_id);
		m_mapIdPlayer.erase(connect_id);
	}
}

void PlayerManager::removePlayer(uint32 player_id)
{
	MapAccountPlayer::iterator it = m_mapAcountIdPlayer.find(player_id);
	if (it != m_mapAcountIdPlayer.end())
	{
		int64 connect_id = it->second->getConnectId();
		string userId = it->second->getUserId();
		delete it->second;
		m_mapAcountIdPlayer.erase(it);
		m_mapIdPlayer.erase(connect_id);
		m_mapUserIdPlayer.erase(userId);
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
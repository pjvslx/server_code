#pragma once

#include <map>
#include <vector>
#include "core/TypeDef.h"
#include "Player.h"
#include "Lock.h"

typedef std::map<int64, Player*> MapIdPlayer;
typedef std::map<uint32, Player*> MapAccountPlayer;
typedef std::map<std::string, Player*> MapUserPlayer;


class PlayerManager
{
public:
	static PlayerManager* getInstance();
	bool init();
	void* removePlayerByConnection(void*obj, void* p);
	void removePlayer(int64 connect_id);
	void removePlayer(uint32 player_id);
	void removePlayer(std::string userId);
	void sendPacket(uint32 playerId,NetPacket* p);
	void sendPacket(int64 connectId,NetPacket* p);
	void sendPacket(const std::vector<uint32>& vecPlayerId,NetPacket* p);
private:
	PlayerManager();
	~PlayerManager();

	void __handlePacket(int64 connectId, unsigned int opcode, void* responseObject);
	void __handleClientLogin(int64 connectId, void* resp);
	void __handleKeepAlive(int64 connectId, void* resp);
	void __handlePlayerInfo(int64 connectId, void* resp);
	char* __encodePacket(NetPacket* p);

	void __kickPlayer(int64 connectId);
private:
	static PlayerManager* m_instance;
	MapIdPlayer m_mapIdPlayer;			//Connect idµΩPlayer”≥…‰
	MapAccountPlayer m_mapAcountIdPlayer;	//account idµΩPlayer”≥…‰
	MapUserPlayer m_mapUserIdPlayer;	//user_id µΩPlayer”≥…‰

	uint32 m_playerSeqId = 0;
};
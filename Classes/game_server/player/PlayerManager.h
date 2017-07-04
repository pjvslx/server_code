#pragma once

#include <map>
#include <vector>
#include "core/TypeDef.h"
#include "Player.h"
#include "Lock.h"

class PlayerManager
{
public:
	static PlayerManager* getInstance();
	bool init();

	void addPlayer(int64 connect_id);
	void removePlayer(int64 connect_id);
	void sendPacket(uint32 playerId,NetPacket* p);
	void sendPacket(int64 connectId,NetPacket* p);
	void sendPacket(const std::vector<uint32>& vecPlayerId,NetPacket* p);
private:
	PlayerManager();
	~PlayerManager();

	void __handlePacket(unsigned int opcode, void* responseObject);
	void __handleClientLogin(void* resp);
	char* __encodePacket(NetPacket* p);
private:
	static PlayerManager* m_instance;
	std::map<int64, Player*> m_mapIdPlayer;			//Connect idµΩPlayer”≥…‰
	std::map<uint32, Player*> m_mapAcountIdPlayer;	//account idµΩPlayer”≥…‰

	int m_playerSeqId = 0;
};
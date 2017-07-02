#pragma once

#include <map>
#include "core/TypeDef.h"
#include "Player.h"
#include "Lock.h"

class PlayerManager
{
public:
	static PlayerManager* getInstance();
	bool init();
private:
	PlayerManager();
	~PlayerManager();

	void __handlePacket(unsigned int opcode, void* responseObject);
	void __handleClientLogin(void* resp);
private:
	static PlayerManager* m_instance;
	std::map<int64, Player*> m_mapIdPlayer;			//Connect idµΩPlayer”≥…‰
	std::map<uint32, Player*> m_mapAcountIdPlayer;	//account idµΩPlayer”≥…‰
	mdk::Mutex m_playerLock;
};
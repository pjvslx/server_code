#pragma once
#include "PlayerDef.h"
#include "core/TypeDef.h"

class Player
{
public:
	Player();
	~Player();
	void setPlayerInfo(const stPlayerAccountCharacterInfo& playerInfo){ m_playerInfo = playerInfo; };
	void setConnectId(int64 id){ m_id = id; };
	int64 getConnectId(){ return m_id; };
	const stPlayerAccountCharacterInfo getPlayerInfo(){ return m_playerInfo; };
private:
	stPlayerAccountCharacterInfo m_playerInfo;
	int64 m_id;
};
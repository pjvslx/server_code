#pragma once
#include "PlayerDef.h"
#include "core/TypeDef.h"

class Player
{
public:
	Player();
	~Player();
private:
	stPlayerAccountCharacterInfo m_playerInfo;
	int64 m_id;
};
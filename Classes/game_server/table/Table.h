#pragma once
#include <vector>
#include "mahjong/MahjongDef.h"
#include "player/Player.h"

class Table
{
public:
	Table();
	~Table();
	void initMahjong();
	bool popMahjong(MahjongID& id);
private:
	int m_tableId;
	int roomId;
	std::vector<MahjongID> m_vecMajhong;
	std::vector<Player*> m_vecPlayer;
};
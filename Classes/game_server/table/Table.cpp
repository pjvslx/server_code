#include "Table.h"
#include <assert.h>
using namespace std;

Table::Table()
{

}

Table::~Table()
{

}

void Table::initMahjong()
{
	m_vecMajhong.clear();

	vector<MahjongID> tmp;
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			tmp.push_back(i);
		}
	}

	for (int i = 11; i <= 19; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			tmp.push_back(i);
		}
	}

	for (int i = 21; i <= 29; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			tmp.push_back(i);
		}
	}

	int count = tmp.size();
	for (int i = 0; i < count; i++)
	{
		int mod = tmp.size();
		int random = rand() % mod;
		int mahjong_id = tmp[random];
		tmp.erase(tmp.begin() + random);
		m_vecMajhong.push_back(mahjong_id);
	}
}

bool Table::popMahjong(MahjongID& id)
{
	if (m_vecMajhong.size() == 0)
	{
		return false;
	}
	id = m_vecMajhong[0];
	m_vecMajhong.erase(m_vecMajhong.begin());
	return true;
}
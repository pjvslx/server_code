#include "Player.h"

Player::Player()
: m_id(-1)
{

}

Player::~Player()
{

}

int Player::getPlayerId()
{
	return m_playerInfo.account.accountID;
}

std::string Player::getUserId()
{
	return m_playerInfo.account.account;
}

void Player::reset()
{

}
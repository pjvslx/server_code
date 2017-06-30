#include "PlayerManager.h"

PlayerManager* PlayerManager::m_instance = nullptr;

PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{

}

PlayerManager* PlayerManager::getInstance()
{
	if (!m_instance)
	{
		m_instance = new PlayerManager();
	}

	return m_instance;
}
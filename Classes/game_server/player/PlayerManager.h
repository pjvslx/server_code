#pragma once

class PlayerManager
{
public:
	static PlayerManager* getInstance();
private:
	PlayerManager();
	~PlayerManager();
	static PlayerManager* m_instance;
};
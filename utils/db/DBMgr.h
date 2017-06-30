#pragma once

class DBMgr
{
public:
	static DBMgr* getInstance();
private:
	static DBMgr* m_instance;
};
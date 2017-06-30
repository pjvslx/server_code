#include "DBMgr.h"

DBMgr* DBMgr::m_instance = nullptr;

DBMgr* DBMgr::getInstance()
{
	if (!m_instance)
	{
		m_instance = new DBMgr();
	}

	return m_instance;
}


#include "AsynTaskManager.h"

using namespace std;

void AsynTaskManager::dealTask()
{
	mdk::AutoLock lock(&m_mutex);
	for (list<AsynGameTask*>::iterator it = m_taskList.begin();
		it != m_taskList.end();)
	{
		if ((*it)->timeout())
		{
			(*it)->execute();
			delete (*it);
			it = m_taskList.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void AsynTaskManager::addTask(AsynGameTask* asynTask)
{
	mdk::AutoLock lock(&m_mutex);
	m_taskList.push_back(asynTask);
}

void AsynTaskManager::removeTask(int taskId)
{
	mdk::AutoLock lock(&m_mutex);
	for (list<AsynGameTask*>::iterator it = m_taskList.begin();
		it != m_taskList.end(); it++)
	{
		if ((*it)->getId() == taskId)
		{
			m_taskList.erase(it);
			break;
		}
	}
}
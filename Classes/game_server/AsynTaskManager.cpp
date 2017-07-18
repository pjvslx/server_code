#include "AsynTaskManager.h"

using namespace std;

void AsynTaskManager::dealTask()
{
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
	m_taskList.push_back(asynTask);
}
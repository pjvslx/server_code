#pragma 
#include <list>
#include "AsynGameTask.h"
#include "core/Singleton.h"

class AsynTaskManager : public Singleton<AsynTaskManager>
{
public:
	AsynTaskManager(){};
	virtual ~AsynTaskManager(){};
public:
	void dealTask();
	void addTask(AsynGameTask* asynTask);
private:
	std::list<AsynGameTask*> m_taskList;
};
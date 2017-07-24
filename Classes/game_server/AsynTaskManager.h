#pragma 
#include <list>
#include "AsynGameTask.h"
#include "core/Singleton.h"
#include "Lock.h"

class AsynTaskManager : public Singleton<AsynTaskManager>
{
public:
	AsynTaskManager(){};
	virtual ~AsynTaskManager(){};
public:
	void dealTask();
	void addTask(AsynGameTask* asynTask);
	void removeTask(int taskId);
private:
	mdk::Mutex m_mutex;
	std::list<AsynGameTask*> m_taskList;
};
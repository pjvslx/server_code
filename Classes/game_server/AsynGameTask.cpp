#include "AsynGameTask.h"
#include <time.h>
#include "AsynTaskManager.h"

int AsynGameTask::m_sequenceId = 0;

AsynGameTask::AsynGameTask()
{
	excuteTime = 0;
	delay_time = 0;
	m_asynTaskId = 0;
}

AsynGameTask::~AsynGameTask()
{

}

void AsynGameTask::delayExecute(int delay_time, mdk::FuntionPointer fun, void *pParam)
{
	m_task.Accept(fun, pParam);
	delay_time = delay_time;
	excuteTime = time(nullptr) + delay_time;
	AsynTaskManager::instance()->addTask(this);
	m_asynTaskId = ++m_sequenceId;
}

void AsynGameTask::delayExecute(int delayTime, mdk::MethodPointer method, void *pObj, void *pParam)
{
	m_task.Accept(method, pObj, pParam);
	delay_time = delayTime;
	int cur_time = time(nullptr);
	excuteTime = cur_time + delayTime;
	AsynTaskManager::instance()->addTask(this);
	m_asynTaskId = ++m_sequenceId;
}

bool AsynGameTask::timeout()
{
	return time(nullptr) >= excuteTime;
}

void AsynGameTask::execute()
{
	m_task.Execute();
}
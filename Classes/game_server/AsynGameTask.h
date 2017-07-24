#pragma once

#include "Task.h"
#include "Executor.h"

class AsynGameTask
{
public:
	AsynGameTask();
	~AsynGameTask();

	void delayExecute(int delayTime, mdk::MethodPointer method, void *pObj, void *pParam);
	void excuteNoDelay(mdk::MethodPointer method, void *pObj, void *pParam);

	void delayExecute(int delay_time, mdk::FuntionPointer fun, void *pParam);
	
	bool timeout();
	int getId(){ return m_asynTaskId; };
protected:
	void execute();
private:
	mdk::Task m_task;
	int excuteTime;
	int delay_time;
	int m_asynTaskId;
	static int m_sequenceId;
	friend class AsynTaskManager;
};
#pragma once

#include <windows.h>
#include <map>
#include <list>
#include "Lock.h"

#pragma once
using namespace std;

//定义一种函数指针
typedef void (*callbackFunc) (void*);
typedef void (*delcallbackFunc) (void*);

typedef struct DBCallBackInfo
{
	callbackFunc pFunc;
	delcallbackFunc pDelFunc;
	void* pParam;
	DBCallBackInfo()
	{
		pFunc = NULL;
		pParam = NULL;
		pDelFunc = NULL;
	}
}DBCallBackInfo;

class DBThread
{
public:
	DBThread(void);
	~DBThread(void);
public:
	void execute(callbackFunc pFunc,delcallbackFunc pDelFunc,void* pParam);
	static DWORD __stdcall run(LPVOID Param);
public:
	HANDLE m_handle;
	DWORD m_id;
	list<DBCallBackInfo> m_listCbInfo;
	mdk::Mutex m_lockCbInfo;
};

class DBHandle
{
public:
	
	void init();
	void initThread(int nNum = 1);
	void execute(callbackFunc pFunc,delcallbackFunc pDelFunc,void* pParam);
public:
	map<int,DBThread*> m_mapThread;
	int m_nSeq;
	int m_nMod;
public:
	static DBHandle* instance();
private:
	static DBHandle* m_instance;
};

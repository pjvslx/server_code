#include "DBHandle.h"
#include "atom.h"

DBHandle* DBHandle::m_instance = NULL;

void DBHandle::init()
{
	m_nSeq = 0;
	initThread(3);
}

DBHandle* DBHandle::instance()
{
	if(!m_instance)
	{
		m_instance = new DBHandle();
	}
	return m_instance;
}

void DBHandle::initThread(int nNum)
{
	//初始化nNum个dbThread线程
	m_nMod = nNum;
	for(int i = 0; i < nNum; i++)
	{
		DBThread* pThread = new DBThread();

		pThread->m_handle = CreateThread(NULL,0,DBThread::run,(LPVOID)pThread,0,&(pThread->m_id));
		m_mapThread.insert(std::map<int,DBThread*>::value_type(i,pThread));
	}
}

void DBHandle::execute(callbackFunc pFunc, delcallbackFunc pDelFunc,void* pParam)
{
	m_nSeq++;
	m_nSeq = m_nSeq/m_nMod;
	//计算当前应该放入到哪个线程处理
	m_mapThread[m_nSeq]->execute(pFunc,pDelFunc,pParam);
}

DBThread::DBThread()
{
}

DBThread::~DBThread()
{

}

DWORD __stdcall DBThread::run(LPVOID Param)
{
	//实际DBThread线程的处理逻辑
	DBThread* pThis = (DBThread*)Param;
	list<DBCallBackInfo> listTmp;
	while(true)
	{
		Sleep(100);
		listTmp.clear();
		mdk::AutoLock autoLock(&(pThis->m_lockCbInfo));
		pThis->m_listCbInfo.swap(listTmp);

		for(list<DBCallBackInfo>::iterator it = listTmp.begin();
			it != listTmp.end(); it++)
		{
			//遍历之前存储的函数以及参数对 并一一执行 
			it->pFunc(it->pParam);
			//释放函数的回调
			it->pDelFunc(it->pParam);
		}
	}
	return 0;
}

void DBThread::execute(callbackFunc pFunc,delcallbackFunc pDelFunc, void *pParam)
{
	//将callbackFunc放入到线程信息列表中
	mdk::AutoLock autoLock(&m_lockCbInfo);
	DBCallBackInfo info;
	info.pFunc = pFunc;
	info.pParam = pParam;
	info.pDelFunc = pDelFunc;
	m_listCbInfo.push_back(info);
	return;

}


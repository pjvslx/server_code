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
	//��ʼ��nNum��dbThread�߳�
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
	//���㵱ǰӦ�÷��뵽�ĸ��̴߳���
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
	//ʵ��DBThread�̵߳Ĵ����߼�
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
			//����֮ǰ�洢�ĺ����Լ������� ��һһִ�� 
			it->pFunc(it->pParam);
			//�ͷź����Ļص�
			it->pDelFunc(it->pParam);
		}
	}
	return 0;
}

void DBThread::execute(callbackFunc pFunc,delcallbackFunc pDelFunc, void *pParam)
{
	//��callbackFunc���뵽�߳���Ϣ�б���
	mdk::AutoLock autoLock(&m_lockCbInfo);
	DBCallBackInfo info;
	info.pFunc = pFunc;
	info.pParam = pParam;
	info.pDelFunc = pDelFunc;
	m_listCbInfo.push_back(info);
	return;

}


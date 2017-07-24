#include "GameServer.h"
#include <stdio.h>
#include <string.h>
#include "json/document.h"
#include "json/writer.h"
#include "core/FileUtil.h"
#include "core/Cast.h"
#include "core/NetPacket.h"
#include "core/Buffer.h"
#include "NetConnect.h"
#include "test/TestDef.h"
#include "MsgDispatcher.h"
#include "player/PlayerManager.h"
#include "AsynTaskManager.h"
#include "OpCode.h"

using namespace std;

GameServer* GameServer::m_instance = nullptr;

GameServer* GameServer::getInstance()
{
	if (!m_instance)
	{
		m_instance = new GameServer;
	}

	SYSTEMTIME time;
	GetLocalTime(&time);
	int64 x = time.wMilliseconds;

	return m_instance;
}

bool GameServer::init()
{
	bool ret = PlayerManager::getInstance()->init();
	if (!ret)
	{
		return ret;
	}

	AsynTaskManager::newInstance();

	return true;
}

GameServer::GameServer()
{
	rapidjson::Document jsonDoc;
	int port = 8888;
	string content = FileUtil::getInstance()->getStringFromFile("../../config/game_server/config.json");
	if (!content.empty())
	{
		jsonDoc.Parse<rapidjson::kParseDefaultFlags>(content.c_str());

		if (jsonDoc.HasMember("port") && jsonDoc["port"].IsNumber())
		{
			auto &fs = jsonDoc["port"];
			port = fs.GetInt();
		}

		if (jsonDoc.HasMember("log_dir") && jsonDoc["log_dir"].IsString())
		{
			auto &fs = jsonDoc["log_dir"];
			string dir = fs.GetString();
			if (dir[dir.length()] != '/')
			{
				dir = dir.append("/");
			}
			FileUtil::getInstance()->setDir(dir);
		}

		if (jsonDoc.HasMember("file_max_capacity") && jsonDoc["file_max_capacity"].IsNumber())
		{
			auto &fs = jsonDoc["file_max_capacity"];
			int max = fs.GetInt();
			FileUtil::getInstance()->setLogMaxCapacity(max);
		}

		FileUtil::getInstance()->writeLog("=======GameServer started=======");
		FileUtil::getInstance()->writeLog("=======GameServer started=======");
		for (int i = 0; i < 10; i++)
		{
			FileUtil::getInstance()->writeLog(std::format("=======GameServer %d", i));
		}
	}
	
	Listen(port);
}

void* GameServer::Main(void* pParam)
{
	while (IsOk())
	{
#ifndef WIN32
		usleep(1000 * 1000);
#else
		
		__mainThreadLoop();
		Sleep(10);
#endif
	}

	return nullptr;
}

void* GameServer::closeConnection(void*obj, void* p)
{
	FileUtil::getInstance()->writeLog("=======GameServer::closeConnection=========");
	int64 id = *(int64*)(p);
	delete p;
	CloseConnect(id);
	return 0;
}

void GameServer::OnConnect(mdk::NetHost &host)
{
	printf("=======GameServer::OnConnect=========\n");
	FileUtil::getInstance()->writeLog("=======GameServer::OnConnect=========");

// 	AsynGameTask *task = new AsynGameTask();
// 	int64* p = new int64(host.ID());
// 	task->delayExecute(5, mdk::Executor::Bind(&GameServer::closeConnection), this, (void*)p);
	NetPacket pack(SMSG_HELLO_CLIENT);
	pack.writeInt(time(nullptr));
	PlayerManager::getInstance()->sendPacket(host.ID(),&pack);
}

void GameServer::OnConnectFailed(char *ip, int port, int reConnectTime)
{
	printf("=======GameServer::OnConnectFailed=========\n");
}

void GameServer::OnCloseConnect(mdk::NetHost &host)
{
	printf("=======GameServer::OnCloseConnect=========\n");
	AsynGameTask *task = new AsynGameTask();
	int64* p = new int64(host.ID());
	task->excuteNoDelay(mdk::Executor::Bind(&PlayerManager::removePlayerByConnection), PlayerManager::getInstance(), (void*)p);
}

void GameServer::OnMsg(mdk::NetHost &host)
{
	printf("=======GameServer::OnMsg=========\n");
	unsigned char c[256] = {0};
	//bool ret = host.Recv(c,10,true);
	//LOG("========GameServer::OnMsg=========== c = %s",c);
	while (true)
	{
		NetPacketHeader header;
		unsigned char c[10*1024] = { 0 };
		//尝试性读取一个包头长度
		bool ret = host.Recv(reinterpret_cast<uint8*>(&header),PACKET_HEADER_SIZE,false);
		if (ret)
		{
			//通过包头去尝试性读取整包
			ret = host.Recv(c, header.packetSize, false);
			if (!ret)
			{
				break;
			}

			header.opcode = _BITSWAP16(header.opcode);
			header.packetSize = _BITSWAP16(header.packetSize);

			if (header.packetSize > sizeof(c))
			{
				host.Close();
				break;
			}

			host.Recv(c, header.packetSize, true);
			NetPacket *p = new NetPacket(header.opcode);
			p->setData(c + PACKET_HEADER_SIZE, header.packetSize - PACKET_HEADER_SIZE);
			p->setConnectId(host.ID());
			try
			{
				//MsgDispatcher::getInstance()->handlePacket(p);
				m_MsgQueue.put(p);
			}
			catch (std::string e)
			{
				host.Close();
				LOG(std::format("exception %s",e.c_str()));
			}
		}
		else
		{
			//包头都不完整
			break;
		}
	}
}

void GameServer::__mainThreadLoop()
{
	__handleBufferQueue();
	AsynTaskManager::instance()->dealTask();
}

void GameServer::__handleBufferQueue()
{
	if (m_MsgQueue.swap())
	{
		NetPacket *packet;
		while (!m_MsgQueue.isEmpty())
		{
			packet = m_MsgQueue.get();
			assert(packet);
			if (packet)
			{
				MsgDispatcher::getInstance()->handlePacket(packet->getConnectId(),packet);
			}
		}
	}
}
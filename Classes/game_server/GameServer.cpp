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
#include "ProtocolFactory.h"
#include "test/TestDef.h"

using namespace std;
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

int GameServer::Main()
{
	while (IsOk())
	{
#ifndef WIN32
		usleep(1000 * 1000);
#else
		Sleep(1000);
#endif
	}
	return 1;
}

void GameServer::OnConnect(mdk::NetHost &host)
{
	printf("=======GameServer::OnConnect=========\n");
}

void GameServer::OnConnectFailed(char *ip, int port, int reConnectTime)
{
	printf("=======GameServer::OnConnectFailed=========\n");
}

void GameServer::OnCloseConnect(mdk::NetHost &host)
{
	printf("=======GameServer::OnCloseConnect=========\n");
}

void GameServer::OnMsg(mdk::NetHost &host)
{
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

			if (header.packetSize > sizeof(c))
			{
				host.Close();
				break;
			}

			host.Recv(c, header.packetSize, true);
			NetPacket p;
			p.setHeaderInfo(header.opcode,header.packetSize);
			p.setData(c + PACKET_HEADER_SIZE, header.packetSize - PACKET_HEADER_SIZE);
			try
			{
				TestMsg msg;
				msg.decode(p);
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
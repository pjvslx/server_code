#include "GameServer.h"
#include <stdio.h>
#include <string.h>
#include "json/document.h"
#include "json/writer.h"
#include "file/FileUtil.h"
#include "file/Cast.h"

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

}
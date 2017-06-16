#include "GameServer.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

GameServer::GameServer()
{
	Listen(8888);
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
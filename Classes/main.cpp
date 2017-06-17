#include <stdio.h>
#include "STNetServer.h"
#include "GameServer.h"
#include "file/FileUtil.h"
#include "file/Cast.h"
void main()
{
	GameServer server;
	server.Start();
	server.WaitStop();
}
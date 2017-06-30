#include <stdio.h>
#include "STNetServer.h"
#include "GameServer.h"
#include "core/FileUtil.h"
#include "core/Cast.h"
void main()
{
	GameServer server;
	server.Start();
	server.WaitStop();
}
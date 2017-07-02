#include <stdio.h>
#include "STNetServer.h"
#include "GameServer.h"
#include "core/FileUtil.h"
#include "core/Cast.h"
#include "MsgDispatcher.h"
void main()
{
	GameServer::getInstance()->Start();
	GameServer::getInstance()->init();
	GameServer::getInstance()->WaitStop();
}
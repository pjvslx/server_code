#include <stdio.h>
#include "STNetServer.h"
#include "GameServer.h"
void main()
{
	printf("=======GameServer started=======\n");
	GameServer server;
	server.Start();
	server.WaitStop();
}
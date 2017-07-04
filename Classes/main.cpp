#include <stdio.h>
#include "STNetServer.h"
#include "GameServer.h"
#include "core/FileUtil.h"
#include "core/Cast.h"
#include "MsgDispatcher.h"
#include "core/TypeDef.h"
void main()
{
	char buffer[1024] = { 0 };
	int input1 = 1, input2 = 2,output1 = 0, output2 = 0;
	printf("sizeofint:%d\n", sizeof(int));

	printf("Pleaseinputtwointegers\n");
	printf("Theinputintegersare:%d %d\n", input1, input2);

	memcpy(buffer, &input1, sizeof(int));
	memcpy(buffer + sizeof(int), &input2, sizeof(int));

	printf("Buffer:\n");
	for (int i  = 0; i < 1024; ++i) {
		printf("0x%02x ", buffer[i]);
	}
	printf("\n");

	memcpy(&output1, buffer, sizeof(int));
	memcpy(&output2, buffer + sizeof(int), sizeof(int));

	printf("Theoutputintegersretrievedfrombuffer:%d%d\n", output1, output2);

	GameServer::getInstance()->Start();
	GameServer::getInstance()->init();
	GameServer::getInstance()->WaitStop();
}
﻿#include <stdio.h>
#include "STNetServer.h"
#include "GameServer.h"
#include "core/FileUtil.h"
#include "core/Cast.h"
#include "MsgDispatcher.h"
#include "core/TypeDef.h"
#include <map>
#include <string>
#include "table/Table.h"
using namespace std;
void main()
{
	srand((int)time(NULL));     //每次执行种子不同，生成不同的随机数
	map<int, string> m;
	m.insert(make_pair(1,"11"));
	m.insert(make_pair(2, "22"));
	m.insert(make_pair(3, "33"));

	m.insert(make_pair(4, "44"));

	m.erase(2);

	Table* p = new Table;
	p->initMahjong();
	

	GameServer::getInstance()->init();
	GameServer::getInstance()->Start();
	GameServer::getInstance()->WaitStop();
}
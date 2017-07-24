#pragma once
enum GAME_OPCODE
{
	SMSG_KEEP_ACTIVE = 33,	//保持连接
	SMSG_HELLO_CLIENT = 6,	//发送给客户端
    SMSG_TEST	= 100,		//连接出错被踢出
	CMSG_ACCOUNT_LOGIN = 101,	//登录请求
	SMSG_LOGIN_RESP,			//登录反馈
	CMSG_PLAYER_INFO,
	SMSG_PLAYER_INFO,			//玩家信息
};
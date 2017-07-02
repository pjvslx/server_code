#pragma once
enum GAME_OPCODE
{
    SMSG_TEST	= 100,		//连接出错被踢出
	CMSG_ACCOUNT_LOGIN = 101,	//登录请求
	SMSG_LOGIN_RESP,			//登录反馈
};
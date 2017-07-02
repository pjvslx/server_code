#include <functional>
#include "PlayerManager.h"
#include "MsgDispatcher.h"
#include "OpCode.h"
#include "PlayerDef.h"
using namespace std;

PlayerManager* PlayerManager::m_instance = nullptr;

PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{

}

bool PlayerManager::init()
{
	MsgDispatcher::getInstance()->addPacketObserver(typeid(this).name(),
		bind(&PlayerManager::__handlePacket, this, placeholders::_1, placeholders::_2)
		, CMSG_ACCOUNT_LOGIN
		, NULL);
	return true;
}

PlayerManager* PlayerManager::getInstance()
{
	if (!m_instance)
	{
		m_instance = new PlayerManager();
	}

	return m_instance;
}

void PlayerManager::__handlePacket(unsigned int opcode, void* resp)
{
	NetPacket *recvPackage = static_cast<NetPacket*>(resp);
	switch (opcode)
	{
	case CMSG_ACCOUNT_LOGIN:
		__handleClientLogin(resp);
		break;
	default:
		break;
	}
}

void PlayerManager::__handleClientLogin(void* resp)
{
	NetPacket* p = static_cast<NetPacket*>(resp);
	C2GMG2GL_AccountLogin accountInfo;
	accountInfo.decode(*p);
}
#include <functional>
#include <map>
#include "STNetServer.h"
#include "STNetHost.h"
#include "NetServer.h"
#include "NetHost.h"
#include "core/DoubleBufferQueue.h"
#include "core/NetPacket.h"

class GameServer : public mdk::NetServer
{
public:
	static GameServer* getInstance();
	bool init();
protected:
	GameServer();
public:
	//override
	void* Main(void* pParam);
	void OnConnect(mdk::NetHost &host);
	void OnConnectFailed(char *ip, int port, int reConnectTime);
	void OnCloseConnect(mdk::NetHost &host);
	void OnMsg(mdk::NetHost &host);

	void __mainThreadLoop();
	void __handleBufferQueue();
private:
	static GameServer* m_instance;
	DoubleBufferQueue<NetPacket*, NetPacket::free> m_MsgQueue;
};
#include <functional>
#include <map>
#include "STNetServer.h"
#include "STNetHost.h"
#include "NetServer.h"
#include "NetHost.h"

class GameServer : public mdk::NetServer
{
public:
	static GameServer* getInstance();
	bool init();
protected:
	GameServer();
public:
	int Main();
	void OnConnect(mdk::NetHost &host);
	void OnConnectFailed(char *ip, int port, int reConnectTime);
	void OnCloseConnect(mdk::NetHost &host);
	void OnMsg(mdk::NetHost &host);
private:
	static GameServer* m_instance;
};
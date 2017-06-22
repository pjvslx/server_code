#pragma once
class NetPacket;

class ProtocolFactory
{
private:
	ProtocolFactory();
	~ProtocolFactory();
public:
	static ProtocolFactory* getInstance();
	NetPacket* createNetPacket(int opCode);
private:
	static ProtocolFactory* m_instance;
};
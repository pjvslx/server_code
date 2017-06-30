#include "ProtocolFactory.h"
#include "core/NetPacket.h"
#include "OpCode.h"
#include "test/TestDef.h"

ProtocolFactory* ProtocolFactory::m_instance = nullptr;

ProtocolFactory::ProtocolFactory()
{

}

ProtocolFactory::~ProtocolFactory()
{

}

ProtocolFactory* ProtocolFactory::getInstance()
{
	if (!m_instance)
	{
		m_instance = new ProtocolFactory();
	}

	return m_instance;
}

NetPacket* ProtocolFactory::createNetPacket(int opCode)
{
	NetPacket* p = nullptr;
	if (SMSG_TEST == opCode)
	{
		//p = new TestMsg();
	}
	return p;
}
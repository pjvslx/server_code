#include "MsgDispatcher.h"
#include <stdarg.h>
#include "core/TypeDef.h"

MsgDispatcher* MsgDispatcher::m_instance = nullptr;

MsgDispatcher* MsgDispatcher::getInstance()
{
	if (!m_instance)
	{
		m_instance = new MsgDispatcher;
	}

	return m_instance;
}

MsgDispatcher::MsgDispatcher()
{

}

void MsgDispatcher::addPacketObserver(const std::string& componentName, const std::function<void(unsigned int, void*)> &func, unsigned int opcode1, ...)
{
	__addObserver(componentName, func, opcode1);

	va_list args;
	va_start(args, opcode1);
	uint32 opcode = va_arg(args, uint32);
	while (opcode)
	{
		__addObserver(componentName, func, opcode);
		opcode = va_arg(args, uint32);
	}

	va_end(args);
}

void MsgDispatcher::__addObserver(const std::string& componentName, const std::function<void(unsigned int, void*)> &func, unsigned int opcode)
{
	m_mapComponentOpcodes[componentName].push_back(opcode);
	m_mapOpcodeFunc[opcode] = func;
}

void MsgDispatcher::removePacketObserver(const std::string&componentName)
{
	for (auto iter : m_mapComponentOpcodes[componentName])
	{
		m_mapOpcodeFunc.erase(iter);
	}

	m_mapComponentOpcodes.erase(componentName);
}

void MsgDispatcher::handlePacket(NetPacket* packet)
{
	uint16 opcode = packet->getOpcode();
	if (m_mapOpcodeFunc.find(opcode) != m_mapOpcodeFunc.end())
	{
		m_mapOpcodeFunc[opcode](opcode, packet);
	}
}
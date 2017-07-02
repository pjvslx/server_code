#pragma once
#include <functional>
#include <map>
#include <vector>
#include "core/NetPacket.h"

class MsgDispatcher
{
public:
	static MsgDispatcher* getInstance();
	void addPacketObserver(const std::string& componentName, const std::function<void(unsigned int, void*)> &func, unsigned int opcode1, ...);
	void removePacketObserver(const std::string&componentName);
	void handlePacket(NetPacket* packet);
protected:
	MsgDispatcher();
private:
	void __addObserver(const std::string& componentName, const std::function<void(unsigned int, void*)> &func, unsigned int opcode);
private:
	static MsgDispatcher* m_instance;
	std::map<std::string, std::vector<unsigned int>> m_mapComponentOpcodes;
	std::map<unsigned int, std::function<void(unsigned int, void*)>> m_mapOpcodeFunc;
};
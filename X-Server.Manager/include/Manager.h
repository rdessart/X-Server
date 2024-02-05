#pragma once

#include <map>
#include <string>
#include <UDPServer.h>

class Manager
{
public:
	Manager(IServer& server, Logger &logger);
	~Manager();

	int AddService(std::string name, void* servicePtr);
	void* GetService(std::string name);

protected:
	IServer m_networking;
	Logger m_logger;
	std::map<std::string, void*> m_serviceMap;
};


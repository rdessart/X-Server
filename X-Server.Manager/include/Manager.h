#pragma once

#include <map>
#include <string>
#include <UDPServer.h>

class Manager
{
public:
	Manager();
	~Manager();

	int AddService(std::string name, void* servicePtr);
	void* GetService(std::string name);
	bool RemoveService(std::string name);

	UDPServer* GetServer();
	Logger* GetLogger();

protected:
	UDPServer m_networking;
	Logger m_logger;
	std::map<std::string, void*> m_serviceMap;
};


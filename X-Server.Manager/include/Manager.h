#pragma once

#include <map>
#include <string>
#include <UDPServer.h>

class Manager
{
public:
	Manager();
	Manager(int sdkVersion);
	~Manager();

	size_t AddService(std::string name, void* servicePtr);
	void* GetService(std::string name);
	bool RemoveService(std::string name);
	bool IsServiceAvailable(std::string name);
	UDPServer* GetServer();
	Logger* GetLogger();
	int GetSDKVersion();
	void SetSDKVersion(int version);
protected:
	UDPServer m_networking;
	Logger m_logger;
	std::map<std::string, void*> m_serviceMap;
	int m_SDKVersion;
};


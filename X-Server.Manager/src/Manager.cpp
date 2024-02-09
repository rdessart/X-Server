#include "../include/Manager.h"

#include "../include/Managers/OperationManager.h"

#define NAMEOF(name) #name
Manager::Manager():
	m_logger("X-Server.log", "Manager", false),
	m_SDKVersion(-1)
{
	m_serviceMap.emplace(NAMEOF(OperationManager), new OperationManager());
}

Manager::Manager(int sdkVersion):
	m_logger("X-Server.log", "Manager", false),
	m_SDKVersion(sdkVersion)
{
	m_serviceMap.emplace(NAMEOF(OperationManager), new OperationManager());
}

Manager::~Manager()
{
	for (auto& kv : m_serviceMap)
	{
		m_logger.Log("Deleting Service : '" + kv.first + "'");
	}
}

size_t Manager::AddService(std::string name, void* servicePtr)
{
	if (m_serviceMap.contains(name))
	{
		m_logger.Log("Service Map already contain a service named '" + name + "' !", Logger::Severity::WARNING);
		return -1;
	}

	m_serviceMap.emplace(name, servicePtr);

	return m_serviceMap.size();
}

void* Manager::GetService(std::string name)
{ 
	if (!m_serviceMap.contains(name))
	{
		m_logger.Log("Service Map don't contain a service named '" + name + "' !", Logger::Severity::WARNING);
		return nullptr;
	}
	return m_serviceMap.at(name);
}

bool Manager::RemoveService(std::string name)
{
	if (!m_serviceMap.contains(name))
	{
		m_logger.Log("Service Map don't contain a service named '" + name + "' !", Logger::Severity::WARNING);
		return false;
	}
	m_serviceMap.erase(name);
	return true;
}

bool Manager::IsServiceAvailable(std::string name)
{
	return m_serviceMap.contains(name);
}

UDPServer* Manager::GetServer()
{
	return &m_networking;
}

Logger* Manager::GetLogger()
{
	return &m_logger;
}

int Manager::GetSDKVersion()
{
	return m_SDKVersion;
}

void Manager::SetSDKVersion(int version)
{
	m_SDKVersion = version;
}

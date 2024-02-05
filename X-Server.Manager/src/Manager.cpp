#include "../include/Manager.h"

Manager::Manager():
	m_logger("X-Server.log", "Manager", false)
{
}

Manager::~Manager()
{
	for (auto& kv : m_serviceMap)
	{
		m_logger.Log("Deleting Service : '" + kv.first + "'");
	}
}

int Manager::AddService(std::string name, void* servicePtr)
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

UDPServer* Manager::GetServer()
{
	return &m_networking;
}

Logger* Manager::GetLogger()
{
	return &m_logger;
}

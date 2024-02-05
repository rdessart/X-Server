#include "../include/Manager.h"

Manager::Manager(IServer& server, Logger& logger) :
	m_networking(server),
	m_logger(logger)
{
}

Manager::~Manager()
{
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

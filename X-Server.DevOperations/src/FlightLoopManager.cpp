#include "FlightLoopManager.h"
#include <Datarefs/AbstractDataref.h>

static float GetterFlightLoopCallback(float _, float __, int ___, void* param)
{
	FlightLoopParameter* parameters = (FlightLoopParameter*)param;
	FlightLoopManager* flm = static_cast<FlightLoopManager*>(parameters->Manager->GetService("FlightLoopManager"));
	std::vector<NamedDataref> datarefs = flm->GetDatarefForFlightLoop(parameters->FlightLoopId);
	json outJson;
	outJson["Operation"] = "CALLBACK_RETURN";
	outJson["CallbackID"] = parameters->FlightLoopId;

	json kvp;
	for (NamedDataref& dataref : datarefs)
	{
		kvp[dataref.first] = dataref.second->GetValue();
	}
	outJson["DatarefsValue"] = kvp;

	Message m = parameters->ReturnMessage;
	m.message = outJson;

	parameters->Manager->GetServer()->SendData(m);

	return parameters->IsTimeReference ? parameters->DeltaTime / 1000.0f : -1.0f * parameters->DeltaTime;
}

FlightLoopManager::FlightLoopManager() :
	m_timeFlightLoops(std::map<unsigned int, unsigned int>()),
	m_frameFlightLoops(std::map<unsigned int, unsigned int>()),
	m_flightLoopsDatarefs(std::map<unsigned int, std::vector<NamedDataref>>()),
	m_lastId(0)
{
}

bool FlightLoopManager::FlightLoopExist(unsigned int deltaTime, bool isTimeReference)
{
	if (isTimeReference) return m_timeFlightLoops.contains(deltaTime);
	return m_frameFlightLoops.contains(deltaTime);
}

bool FlightLoopManager::FlightLoopExist(unsigned int id)
{
	return id <= m_lastId;
}

unsigned int FlightLoopManager::GetFlightLoop(unsigned int deltaTime, bool isTimeReference, Manager* mastercallback, const Message& message)
/// <summary>
/// Create or return the flightloop id of a requested flightloop
/// </summary>
{
	if (isTimeReference && m_timeFlightLoops.contains(deltaTime)) return m_timeFlightLoops[deltaTime];
	else if (!isTimeReference && m_frameFlightLoops.contains(deltaTime)) return m_frameFlightLoops[deltaTime];

	return RegisterFlightLoop(deltaTime, isTimeReference, mastercallback, message);
}


bool FlightLoopManager::AssignDatarefToFlightLoop(unsigned int flightloopId, std::string name, AbstractDataref* dataref)
{
	if (!m_flightLoopsDatarefs.contains(flightloopId)) return false;
	m_flightLoopsDatarefs[flightloopId].push_back(NamedDataref(name, dataref));
	return true;
}

bool FlightLoopManager::AssignDatarefToFlightLoop(unsigned int flightloopId, NamedDataref dataref)
{
	if (!m_flightLoopsDatarefs.contains(flightloopId)) return false;
	m_flightLoopsDatarefs[flightloopId].push_back(dataref);
	return true;
}

std::vector<NamedDataref> FlightLoopManager::GetDatarefForFlightLoop(unsigned int flightLoopid)
{
	return m_flightLoopsDatarefs.at(flightLoopid);
}

unsigned int FlightLoopManager::RegisterFlightLoop(unsigned int deltaTime, bool isTimeReference, Manager* manager, const Message& message)
{
	m_lastId++;
	FlightLoopParameter* parameters = new FlightLoopParameter();
	parameters->DeltaTime = deltaTime;
	parameters->IsTimeReference = isTimeReference;
	parameters->Manager = manager;
	parameters->FlightLoopId = m_lastId;
	parameters->ReturnMessage = message;

	if (isTimeReference) {
		XPLMRegisterFlightLoopCallback(GetterFlightLoopCallback, (float)deltaTime / 1000.0f, parameters);
		m_timeFlightLoops.emplace(deltaTime, m_lastId);
	}
	else {
		XPLMRegisterFlightLoopCallback(GetterFlightLoopCallback, -1.0f * (float)deltaTime, parameters);
		m_frameFlightLoops.emplace(deltaTime, m_lastId);
	}
	m_flightLoopsDatarefs.emplace(m_lastId, std::vector<NamedDataref>());
	return m_lastId;
}

unsigned int FlightLoopManager::UnregisterFlightLoop(unsigned int deltaTime, bool isTimeReference)
{
	return 0;
}

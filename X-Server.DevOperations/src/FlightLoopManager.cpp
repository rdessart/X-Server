#include "FlightLoopManager.h"
#include <Datarefs/AbstractDataref.h>

static float GetterFlightLoopCallback(float _, float __, int ___, void* param)
{
	FlightLoopParameter* parameters = (FlightLoopParameter*)param;
	FlightLoopManager* flm = static_cast<FlightLoopManager*>(parameters->Manager->GetService("FlightLoopManager"));
	std::vector<NamedDataref> datarefs = flm->GetDatarefForFlightLoop(parameters->FlightLoopId);
	json outJson;
	outJson["Operation"] = "CALLBACK_RETURN";
	outJson["CallbackID"] = (intptr_t)(parameters->FlightLoopId);

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

FlightLoopManager::FlightLoopManager()
{
}

bool FlightLoopManager::FlightLoopExist(timerInfo deltaTime, bool isTimeReference)
{
	if (isTimeReference) return m_timeFlightLoops.contains(deltaTime);
	return m_frameFlightLoops.contains(deltaTime);
}

bool FlightLoopManager::FlightLoopExist(XPLMFlightLoopID id)
{
	return GetFlightLoop(id) != 0;
}

XPLMFlightLoopID FlightLoopManager::GetFlightLoop(timerInfo deltaTime, bool isTimeReference, Manager* mastercallback, const Message& message)
/// <summary>
/// Create or return the flightloop id of a requested flightloop
/// </summary>
{
	if (isTimeReference && m_timeFlightLoops.contains(deltaTime)) return m_timeFlightLoops[deltaTime];
	else if (!isTimeReference && m_frameFlightLoops.contains(deltaTime)) return m_frameFlightLoops[deltaTime];

	return RegisterFlightLoop(deltaTime, isTimeReference, mastercallback, message);
}


bool FlightLoopManager::AssignDatarefToFlightLoop(XPLMFlightLoopID flightloopId, std::string name, AbstractDataref* dataref)
{
	if (!m_flightLoopsDatarefs.contains(flightloopId)) return false;
	m_flightLoopsDatarefs[flightloopId].push_back(NamedDataref(name, dataref));
	return true;
}

bool FlightLoopManager::AssignDatarefToFlightLoop(XPLMFlightLoopID flightloopId, NamedDataref dataref)
{
	if (!m_flightLoopsDatarefs.contains(flightloopId)) return false;
	m_flightLoopsDatarefs[flightloopId].push_back(dataref);
	return true;
}

bool FlightLoopManager::UnassignDatarefToFlightLoop(XPLMFlightLoopID flightloopId, std::string name)
{
	if (!m_flightLoopsDatarefs.contains(flightloopId)) return false;
	auto it = m_flightLoopsDatarefs[flightloopId].begin();
	bool found = false;
	while (++it != m_flightLoopsDatarefs[flightloopId].end())
	{
		if (it->first == name)
		{
			found = true;
			break;
		}
	}
	if (found)
	{
		delete it->second;
		m_flightLoopsDatarefs[flightloopId].erase(it);
	}
	return found;
}

bool FlightLoopManager::DeleteFlightLoop(XPLMFlightLoopID flightloopId)
{
	int time = GetFlightLoop(flightloopId);
	if (time == 0) return false;
	if (time > 0)
	{
		XPLMDestroyFlightLoop(m_timeFlightLoops[time]);
		m_timeFlightLoops.erase(time); 
	}
	else
	{
		XPLMDestroyFlightLoop(m_frameFlightLoops[time * -1]);
		m_frameFlightLoops.erase(time * -1);
	}

	for (auto it = m_flightLoopsDatarefs.begin(); it != m_flightLoopsDatarefs.end();)
	{
		if (it->first == flightloopId)
		{
			while (it->second.size() > 0)
			{
				delete it->second.begin()->second;
				it->second.erase(it->second.begin());
			}
			m_flightLoopsDatarefs.erase(it);
		}
	}
	return true;
}

std::vector<NamedDataref> FlightLoopManager::GetDatarefForFlightLoop(XPLMFlightLoopID flightLoopid)
{
	return m_flightLoopsDatarefs[flightLoopid];
}

XPLMFlightLoopID FlightLoopManager::RegisterFlightLoop(unsigned int deltaTime, bool isTimeReference, Manager* manager, const Message& message)
{
	FlightLoopParameter* parameters = new FlightLoopParameter();
	parameters->DeltaTime = deltaTime;
	parameters->IsTimeReference = isTimeReference;
	parameters->Manager = manager;
	parameters->FlightLoopId = 0;
	parameters->ReturnMessage = message;

	XPLMCreateFlightLoop_t flightLoopParam;
	flightLoopParam.structSize = static_cast<int>(sizeof(XPLMCreateFlightLoop_t));
	flightLoopParam.phase = xplm_FlightLoop_Phase_AfterFlightModel;
	flightLoopParam.callbackFunc = GetterFlightLoopCallback;
	flightLoopParam.refcon = parameters;

	XPLMFlightLoopID id = XPLMCreateFlightLoop(&flightLoopParam);
	parameters->FlightLoopId = id;
	if (isTimeReference) {
		XPLMScheduleFlightLoop(id, (float)deltaTime / 1000.0f, 1);
		m_timeFlightLoops.emplace(deltaTime, id);
	}
	else {
		XPLMScheduleFlightLoop(id, -1.0f * (float)deltaTime, 1);
		m_frameFlightLoops.emplace(deltaTime, id);
	}
	m_flightLoopsDatarefs.emplace(id, std::vector<NamedDataref>());
	return id;
}

XPLMFlightLoopID FlightLoopManager::UnregisterFlightLoop(unsigned int deltaTime, bool isTimeReference)
{
	return 0;
}

int FlightLoopManager::GetFlightLoop(XPLMFlightLoopID id)
{
	for(auto it_time = m_timeFlightLoops.begin(); it_time != m_timeFlightLoops.end();)
	{
		if (it_time->second == id)
			return it_time->first;
	}

	for (auto it_frame = m_frameFlightLoops.begin(); it_frame != m_frameFlightLoops.end();)
	{
		if (it_frame->second == id)
			return -1 * it_frame->first;
	}

	return 0;
}

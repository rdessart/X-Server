#pragma once

#include <UDPServer.h>
#include <map>
#include <vector>

#include <nlohmann/json.hpp>

#include <XPLM/XPLMProcessing.h>

#include <Message.h>

#include "FlightLoopParameters.h"

using json = nlohmann::json;

typedef std::pair<std::string, class AbstractDataref*> NamedDataref;
typedef unsigned int timerInfo;
class FlightLoopManager 
{
/// <summary>
/// This class goal is :
///  - create flightloop callback (setters, getters).
///  - manage storage (register, unregister).
///  - create link between datarefs and flightloops
/// </summary>
public:
	FlightLoopManager();
	bool FlightLoopExist(timerInfo, bool isTimeReference);
	bool FlightLoopExist(XPLMFlightLoopID id);
	XPLMFlightLoopID GetFlightLoop(timerInfo deltaTime, bool isTimeReference, Manager* manager, const Message &message);
	bool AssignDatarefToFlightLoop(XPLMFlightLoopID flightloopId, std::string name, class AbstractDataref* dataref);
	bool AssignDatarefToFlightLoop(XPLMFlightLoopID flightloopId, NamedDataref dataref);
	bool UnassignDatarefToFlightLoop(XPLMFlightLoopID flightloopId, std::string name);
	bool DeleteFlightLoop(XPLMFlightLoopID flightloopId);
	std::vector<NamedDataref> GetDatarefForFlightLoop(XPLMFlightLoopID flightLoopid);

protected:
	std::map<timerInfo, XPLMFlightLoopID> m_timeFlightLoops;
	std::map<timerInfo, XPLMFlightLoopID> m_frameFlightLoops;
	std::map<XPLMFlightLoopID, std::vector<NamedDataref>> m_flightLoopsDatarefs;

	XPLMFlightLoopID RegisterFlightLoop(unsigned int deltaTime, bool isTimeReference, Manager* manager, const Message& message);
	XPLMFlightLoopID UnregisterFlightLoop(unsigned int deltaTime, bool isTimeReference);

	int GetFlightLoop(XPLMFlightLoopID id);
};

// TODO: As Callbacks are linked to the sender, we could juste use and uint as Id. Multiples recepients could have their own callbacks
// TODO: Possibility to unreference flightloops
// TODO: Possibility to unreference datarefs
// TODO: Create a destructor to clean up the mess.
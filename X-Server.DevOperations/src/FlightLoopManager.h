#pragma once

#include <UDPServer.h>
#include <map>
#include <vector>

#include <nlohmann/json.hpp>

#include <XPLM/XPLMProcessing.h>
#include <XPLM/XPLMDataAccess.h>

#include <Message.h>

#include "FlightLoopParameters.h"

using json = nlohmann::json;

typedef std::pair<std::string, XPLMDataRef> NamedDataref;

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
	bool FlightLoopExist(unsigned int deltaTime, bool isTimeReference);
	bool FlightLoopExist(unsigned int id);
	unsigned int GetFlightLoop(unsigned int deltaTime, bool isTimeReference, Manager* manager, const Message &message);
	bool AssignDatarefToFlightLoop(unsigned int flightloopId, std::string name, XPLMDataRef dataref);
	bool AssignDatarefToFlightLoop(unsigned int flightloopId, NamedDataref dataref);
	std::vector<NamedDataref> GetDatarefForFlightLoop(unsigned int flightLoopid);

protected:
	std::map<unsigned int, unsigned int> m_timeFlightLoops;
	std::map<unsigned int, unsigned int> m_frameFlightLoops;
	std::map<unsigned int, std::vector<NamedDataref>> m_flightLoopsDatarefs;
	unsigned int m_lastId;

	unsigned int RegisterFlightLoop(unsigned int deltaTime, bool isTimeReference, Manager* manager, const Message& message);
	unsigned int UnregisterFlightLoop(unsigned int deltaTime, bool isTimeReference);
};

// TODO: As Callbacks are linked to the sender, we could juste use and uint as Id. Multiples recepients could have their own callbacks
// TODO: Possibility to unreference flightloops
// TODO: Possibility to unreference datarefs
// TODO: Create a destructor to clean up the mess.
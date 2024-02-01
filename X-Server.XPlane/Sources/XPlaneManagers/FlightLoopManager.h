#pragma once
#include "../MasterCallback.h"

#include <map>
#include <vector>

#include <nlohmann/json.hpp>

#include <XPLM/XPLMProcessing.h>


#include "../Datarefs/AbstractDataref.h"

using json = nlohmann::json;

typedef std::pair<std::string, AbstractDataref*> NamedDataref;

struct FlightLoopParameter {
	unsigned int DeltaTime;
	bool IsTimeReference;
	unsigned int FlightLoopId;
	struct MasterCallbackParameter* MasterCallbackParameters;
	Message ReturnMessage;
};

class FlightLoopManager {
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
	unsigned int GetFlightLoop(unsigned int deltaTime, bool isTimeReference, MasterCallbackParameter* mastercallback, const Message &message);
	bool AssignDatarefToFlightLoop(unsigned int flightloopId, std::string name, AbstractDataref* dataref);
	bool AssignDatarefToFlightLoop(unsigned int flightloopId, NamedDataref dataref);
	std::vector<NamedDataref> GetDatarefForFlightLoop(unsigned int flightLoopid);

protected:
	std::map<unsigned int, unsigned int> m_timeFlightLoops;
	std::map<unsigned int, unsigned int> m_frameFlightLoops;
	std::map<unsigned int, std::vector<NamedDataref>> m_flightLoopsDatarefs;
	unsigned int m_lastId;

	unsigned int RegisterFlightLoop(unsigned int deltaTime, bool isTimeReference, MasterCallbackParameter* mastercallback, const Message& message);
	unsigned int UnregisterFlightLoop(unsigned int deltaTime, bool isTimeReference);
};
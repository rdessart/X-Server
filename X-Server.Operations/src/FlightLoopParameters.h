#pragma once

#include <Manager.h>

struct FlightLoopParameter {
	unsigned int DeltaTime;
	bool IsTimeReference;
	XPLMFlightLoopID FlightLoopId;
	class Manager* Manager;
	Message ReturnMessage;
};
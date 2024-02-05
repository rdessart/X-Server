#pragma once

#include <Manager.h>

struct FlightLoopParameter {
	unsigned int DeltaTime;
	bool IsTimeReference;
	unsigned int FlightLoopId;
	class Manager* Manager;
	Message ReturnMessage;
};
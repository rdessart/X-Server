#pragma once

#include "OperationParameters.h"

struct FlightLoopParameter {
	unsigned int DeltaTime;
	bool IsTimeReference;
	unsigned int FlightLoopId;
	struct OperationParameters* MasterCallbackParameters;
	Message ReturnMessage;
};
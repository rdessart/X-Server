#pragma once

#include <Logger.h>
#include <Message.h>
#include <OperationParameters.h>

#ifdef OPERATIONS_EXPORT
#define OPERATION_API extern "C" __declspec(dllexport)
#else
#define OPERATION_API extern "C" __declspec(dllimport)
#endif

OPERATION_API int GetOperations(std::map<std::string, std::string>* operationsNames);

OPERATION_API void RegisterFlightLoopOperation(Message& m, OperationParameters* parameters);
OPERATION_API void SubscribeDatarefOperation(Message& m, OperationParameters* parameters);
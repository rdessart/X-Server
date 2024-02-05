#pragma once

#include <Logger.h>
#include <Message.h>
#include <Manager.h>

#ifdef IBM
    #ifdef OPERATIONS_EXPORT
        #define OPERATION_API extern "C" __declspec(dllexport)
    #else
        #define OPERATION_API extern "C" __declspec(dllimport)
    #endif
#else
    #define OPERATION_API extern "C" 
#endif

OPERATION_API int InitializeDLL(Manager* manager);
OPERATION_API int UninitializeDLL(Manager* manager);
OPERATION_API int GetOperations(std::map<std::string, std::string>* operationsNames);

OPERATION_API void RegisterFlightLoopOperation(Message& m, Manager* manager);
//OPERATION_API void SubscribeDatarefOperation(Message& m, Manager* manager);
#include "Operations.h"

#include <XPLM/XPLMUtilities.h>
#include <Managers/DatarefManager.h>
#include <Managers/FlightLoopManager.h>
#include <Datarefs/AbstractDataref.h>
#include <Datarefs/Dataref.h>
#include <Datarefs/FFA320Dataref.h>

#define NAMEOF(name) #name

OPERATION_API int GetOperations(std::map<std::string, std::string>* operationsNames)
{
    if (operationsNames == nullptr) return -1;
    size_t sizeBefore = operationsNames->size();
    operationsNames->emplace("speak2",          NAMEOF(SpeakOperation));
    operationsNames->emplace("regflightloop",  NAMEOF(RegisterFlightLoopOperation));
    operationsNames->emplace("subdata",        NAMEOF(SubscribeDatarefOperation));

    return (int)(operationsNames->size() - sizeBefore);
}

OPERATION_API void SpeakOperation(Message& m, OperationParameters* parameters)
{
    XPLMSpeakString("Speaking from operations beta version 2");
    m.message["Result"] = "Ok";
}

OPERATION_API void RegisterFlightLoopOperation(Message& m, OperationParameters* parameters)
/// <summary>
/// Required Fields:
///     json : CallbackInfo {
///         INT  : DeltaTime,
///         BOOL : IsTime 
///     }
/// </summary>
{
    if (!m.message.contains("CallbackInfo"))
    {
        m.message["Result"] = "Error:Missing Required CallbackInfo Section";
        return;
    }
    unsigned int deltatime = m.message["CallbackInfo"]["DeltaTime"].get<unsigned int>();
    bool timeRelative = m.message["CallbackInfo"]["IsTime"].get<bool>();
    unsigned int flightloopId = 0;

    if (!parameters->FlightLoopManager->FlightLoopExist(deltatime, timeRelative))
    {
        Message m2;
        m2.target = m.target;
        m2.target_lenght = m.target_lenght;
        flightloopId = parameters->FlightLoopManager->GetFlightLoop(deltatime, timeRelative, parameters, m2);
    }
    else
        flightloopId = parameters->FlightLoopManager->GetFlightLoop(deltatime, timeRelative, nullptr, m);
    m.message["Result"] = "Ok";
    m.message["CallbackId"] = flightloopId;
}

OPERATION_API void SubscribeDatarefOperation(Message& m, OperationParameters* parameters)
{
    if (!m.message.contains("CallbackId"))
    {
        m.message["Result"] = "Error:Missing Required CallbackId";
        return;
    }

    if (!m.message.contains("Name"))
    {
        m.message["Result"] = "Error:Missing Required Dataref Name";
        return;
    }
    std::string datarefName = m.message["Name"].get<std::string>();
    AbstractDataref* dataref = parameters->DatarefManager->GetDatarefByName(datarefName);
    unsigned int callbackId = m.message["CallbackId"].get<unsigned int>();
    if (dataref == nullptr)
    {
        m.message["Result"] = "Error:Dataref was not registered before call";
        return;
    }
    if (!parameters->FlightLoopManager->FlightLoopExist(callbackId))
    {
        m.message["Result"] = "Error:callback not found";
        return;
    }

    parameters->FlightLoopManager->AssignDatarefToFlightLoop(callbackId, datarefName, dataref);
    m.message["Result"] = "Ok";
    return;
}
#include "Operations.h"

#include <XPLM/XPLMUtilities.h>
#include "FlightLoopManager.h"
#include <Datarefs/AbstractDataref.h>
#include <Datarefs/Dataref.h>
#include <Datarefs/FFA320Dataref.h>

#define NAMEOF(name) #name

OPERATION_API int InitializeDLL(Manager* manager)
{
    return manager->AddService(NAMEOF(FlightLoopManager), new FlightLoopManager());
}

OPERATION_API int UninitializeDLL(Manager* manager)
{
    return (int)manager->RemoveService(NAMEOF(FlightLoopManager));
}

OPERATION_API int GetOperations(std::map<std::string, std::string>* operationsNames)
{
    if (operationsNames == nullptr) return -1;
    size_t sizeBefore = operationsNames->size();
    operationsNames->emplace("speak2",          NAMEOF(SpeakOperation));
    operationsNames->emplace("regflightloop",   NAMEOF(RegisterFlightLoopOperation));
    operationsNames->emplace("subdata",         NAMEOF(SubscribeDatarefOperation));
    operationsNames->emplace("unsubdata",       NAMEOF(UnsubscribeDatarefOperation));
    operationsNames->emplace("unregflightloop", NAMEOF(UnregisterFlightLoopOperation));

    return (int)(operationsNames->size() - sizeBefore);
}

OPERATION_API void SpeakOperation(Message& m, Manager* parameters)
{
    XPLMSpeakString("Speaking from operations beta version 2");
    m.message["Result"] = "Ok";
}

OPERATION_API void RegisterFlightLoopOperation(Message& m, Manager* manager)
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
    intptr_t flightloopId = 0;
    FlightLoopManager* flm = static_cast<FlightLoopManager*>(manager->GetService(NAMEOF(FlightLoopManager)));
    if (!flm->FlightLoopExist(deltatime, timeRelative))
    {
        Message m2;
        m2.target = m.target;
        m2.target_lenght = m.target_lenght;
        flightloopId = (intptr_t)(flm->GetFlightLoop(deltatime, timeRelative, manager, m2));
    }
    else
        flightloopId = (intptr_t)(flm->GetFlightLoop(deltatime, timeRelative, nullptr, m));
    m.message["Result"] = "Ok";
    m.message["CallbackId"] = flightloopId;
}

OPERATION_API void SubscribeDatarefOperation(Message& m, Manager* manager)
{
   if (!m.message.contains("CallbackId"))
   {
       m.message["Result"] = "Error:Missing Required CallbackId";
       return;
   }

   if (!m.message.contains("Dataref"))
   {
       m.message["Result"] = "Error:Missing Required Dataref Section";
       return;
   }

   if (!m.message.contains("Name"))
   {
       m.message["Result"] = "Error:Missing Required Dataref Name";
       return;
   }

   std::string datarefName = m.message["Name"].get<std::string>();
   AbstractDataref* dataref = new Dataref();
   dataref->FromJson(m.message["Dataref"]);
   FlightLoopManager* flm = static_cast<FlightLoopManager*>(manager->GetService(NAMEOF(FlightLoopManager)));
   XPLMFlightLoopID callbackId = (XPLMFlightLoopID)m.message["CallbackId"].get<intptr_t>();

   if (dataref == nullptr)
   {
       m.message["Result"] = "Error:Dataref was not registered before call";
       return;
   }
   if (!flm->FlightLoopExist(callbackId))
   {
       m.message["Result"] = "Error:callback not found";
       return;
   }

   flm->AssignDatarefToFlightLoop(callbackId, datarefName, dataref);
   m.message["Result"] = "Ok";
   return;
}

void UnsubscribeDatarefOperation(Message& m, Manager* manager)
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
    XPLMFlightLoopID callbackId = (XPLMFlightLoopID)m.message["CallbackId"].get<intptr_t>();
    FlightLoopManager* flm = static_cast<FlightLoopManager*>(manager->GetService(NAMEOF(FlightLoopManager)));
    if (!flm->UnassignDatarefToFlightLoop(callbackId, datarefName))
    {
        m.message["Result"] = "Error:Unexceptected event while unregistering dataref";
        return;
    }
    m.message["Result"] = "Ok";
}

void UnregisterFlightLoopOperation(Message& m, Manager* manager)
{
    if (!m.message.contains("CallbackId"))
    {
        m.message["Result"] = "Error:Missing Required CallbackId";
        return;
    }

    XPLMFlightLoopID callbackId = (XPLMFlightLoopID)m.message["CallbackId"].get<intptr_t>();
    FlightLoopManager* flm = static_cast<FlightLoopManager*>(manager->GetService(NAMEOF(FlightLoopManager)));
    if (!flm->DeleteFlightLoop(callbackId))
    {
        m.message["Result"] = "Error:Unexceptected event while unregistering dataref";
        return;
    }
    m.message["Result"] = "Ok";
}

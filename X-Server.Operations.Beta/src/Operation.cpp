#include "Operation.h"

#include <XPLM/XPLMPlanes>

OPERATION_API int InitializeDLL(Manager *manager)
{
    return 1;
}

OPERATION_API int UninitializeDLL(Manager *manager)
{
    return 1;
}

OPERATION_API int GetOperations(std::map<std::string, std::string> *operationsNames)
{
    if (operationsNames == nullptr) return -1;
    size_t sizeBefore = operationsNames->size();
    operationsNames->emplace("speak",           NAMEOF(SpeakOperation));
    operationsNames->emplace("setdata",         NAMEOF(SetDatarefOperation));
    operationsNames->emplace("getdata",         NAMEOF(GetDatarefOperation));
    operationsNames->emplace("regdata",         NAMEOF(RegisterDatarefOperation));
    operationsNames->emplace("setregdata",      NAMEOF(SetRegisteredDatarefOperation));
    operationsNames->emplace("getregdata",      NAMEOF(GetRegisteredDatarefOperation));
    operationsNames->emplace("datainfo",        NAMEOF(GetDatarefInfoOperation));
    operationsNames->emplace("regdatainfo",     NAMEOF(GetRegisteredDatarefInfoOperation));
    operationsNames->emplace("regflightloop",   NAMEOF(RegisterFlightLoopOperation));
    operationsNames->emplace("subdata",         NAMEOF(SubscribeDatarefOperation));
    operationsNames->emplace("unsubdata",       NAMEOF(UnsubscribeDatarefOperation));
    operationsNames->emplace("unregflightloop", NAMEOF(UnregisterFlightLoopOperation));
    return (int)(operationsNames->size() - sizeBefore);
}

OPERATION_API void AquirePlanes(Message &message, Manager *manager)
{
    //TODO: check if we can pass a callback to be called when we have plane acess.
    int res = XPLMAcquirePlanes(nullptr, nullptr, nullptr);
    if(!res)
    {
        message.message["Result"] = "Error:Unable to get plane aquisition()";
        return;
    }
    Dataref dataref = Dataref();
    dataref.Load("sim/operation/override/override_TCAS");
    dataref.SetValue("1");
    message.message["Result"] = "Ok";
}

OPERATION_API void ReleasePlanes(Message &message, Manager *manager)
{
    XPLMReleasePlanes();
    message.message["Result"] = "Ok";
}

OPERATION_API void SetPlanesCount(Message &message, Manager *manager)
{
    XPLMSetActiveAircraftCount(message.message["Planes"].get<int>());
    message.message["Result"] = "Ok";
}

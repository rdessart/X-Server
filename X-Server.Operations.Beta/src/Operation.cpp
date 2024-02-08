#include "Operation.h"

#include <XPLM/XPLMPlanes.h>
#include <Datarefs/Dataref.h>

#define NAMEOF(name) #name

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
    operationsNames->emplace("aquireplanes",          NAMEOF(AquirePlanes));
    operationsNames->emplace("releaseplanes",         NAMEOF(ReleasePlanes));
    operationsNames->emplace("setplanecount",         NAMEOF(SetPlanesCount));
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
    Dataref dataref;
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
    XPLMSetActiveAircraftCount(message.message["Count"].get<int>());
    message.message["Result"] = "Ok";
}

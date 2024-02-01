#pragma once
#include <UDPServer.h>

#include "XPlaneManagers/DatarefManager.h"

struct MasterCallbackParameter 
{
    UDPServer* Server;
    class DatarefManager* DatarefManager;
    Logger* Logger;
};

typedef void(*callbackFunction)(Message&, MasterCallbackParameter*);


void Callback(double step, void* tag);
void SpeakOperation(Message& message, MasterCallbackParameter* parameters);
void SetDatarefOperation(Message& message, MasterCallbackParameter* parameters);
void GetDatarefOperation(Message& message, MasterCallbackParameter* parameters);
void RegisterDatarefOperation(Message& message, MasterCallbackParameter* parameters);
void SetRegisteredDatarefOperation(Message& message, MasterCallbackParameter* parameters);
void GetRegisteredDatarefOperation(Message& message, MasterCallbackParameter* parameters);
void GetDatarefInfoOperation(Message& message, MasterCallbackParameter* parameters);
void GetRegisteredDatarefInfoOperation(Message& message, MasterCallbackParameter* parameters);

struct CallbackOperations
{
    static std::map<std::string, callbackFunction> GetOperationsStrings()
    {
        return std::map<std::string, callbackFunction>
        {
            {"speak", SpeakOperation},
            { "setdata", SetDatarefOperation },
            { "getdata", GetDatarefOperation },
            { "registerdata", RegisterDatarefOperation },
            { "setregdata", SetRegisteredDatarefOperation },
            { "getregdata", GetRegisteredDatarefOperation },
            { "datarefinfo", GetDatarefInfoOperation },
            { "regdatarefinfo", GetRegisteredDatarefInfoOperation },
        };
    };

    static callbackFunction GetOperation(std::string ops)
    {
        auto opsMap = GetOperationsStrings();
        std::transform(ops.begin(), ops.end(), ops.begin(),
            [](unsigned char c) { return std::tolower(c); });
        auto itr = opsMap.find(ops);
        if (itr != opsMap.end())
        {
            return itr->second;
        }
        return nullptr;
    }
};
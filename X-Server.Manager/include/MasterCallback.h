#pragma once
#include <UDPServer.h>

#include "Managers/DatarefManager.h"
#include "Managers/FlightLoopManager.h"
#include "OperationParameters.h"

typedef void(*OperationPointer)(Message&, OperationParameters*);


void Callback(double step, void* tag);
void SpeakOperation(Message& message, OperationParameters* parameters);
void SetDatarefOperation(Message& message, OperationParameters* parameters);
void GetDatarefOperation(Message& message, OperationParameters* parameters);
void RegisterDatarefOperation(Message& message, OperationParameters* parameters);
void SetRegisteredDatarefOperation(Message& message, OperationParameters* parameters);
void GetRegisteredDatarefOperation(Message& message, OperationParameters* parameters);
void GetDatarefInfoOperation(Message& message, OperationParameters* parameters);
void GetRegisteredDatarefInfoOperation(Message& message, OperationParameters* parameters);
void RegisterFlightLoopOperation(Message& m, OperationParameters* parameters);
void SubscribeDatarefOperation(Message& m, OperationParameters* parameters);

struct CallbackOperations
{
    static std::map<std::string, OperationPointer> GetOperationsStrings()
    {
        return std::map<std::string, OperationPointer>
        {
            {"speak", SpeakOperation},
            { "setdata", SetDatarefOperation },
            { "getdata", GetDatarefOperation },
            { "registerdata", RegisterDatarefOperation },
            { "setregdata", SetRegisteredDatarefOperation },
            { "getregdata", GetRegisteredDatarefOperation },
            { "datarefinfo", GetDatarefInfoOperation },
            { "regdatarefinfo", GetRegisteredDatarefInfoOperation },
            { "regflightloop", RegisterFlightLoopOperation },
            { "subdataref", SubscribeDatarefOperation },
        };
    };

    static OperationPointer GetOperation(std::string ops)
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
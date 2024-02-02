#pragma once
#include <map>
#include <string>

#include <UDPServer.h>

#include "Managers/DatarefManager.h"
#include "Managers/FlightLoopManager.h"
#include "OperationParameters.h"

typedef void(*OperationPointer)(Message&, OperationParameters*);
typedef int(*OperationLoaderPointer)(std::map<std::string, std::string>*);

void Callback(double step, void* tag);

//struct CallbackOperations
//{
//    static std::map<std::string, OperationPointer> GetOperationsStrings()
//    {
//        return std::map<std::string, OperationPointer>
//        {
//            {"speak", SpeakOperation},
//            { "setdata", SetDatarefOperation },
//            { "getdata", GetDatarefOperation },
//            { "registerdata", RegisterDatarefOperation },
//            { "setregdata", SetRegisteredDatarefOperation },
//            { "getregdata", GetRegisteredDatarefOperation },
//            { "datarefinfo", GetDatarefInfoOperation },
//            { "regdatarefinfo", GetRegisteredDatarefInfoOperation },
//            { "regflightloop", RegisterFlightLoopOperation },
//            { "subdataref", SubscribeDatarefOperation },
//        };
//    };
//
//    static OperationPointer GetOperation(std::string ops)
//    {
//        auto opsMap = GetOperationsStrings();
//        std::transform(ops.begin(), ops.end(), ops.begin(),
//            [](unsigned char c) { return std::tolower(c); });
//        auto itr = opsMap.find(ops);
//        if (itr != opsMap.end())
//        {
//            return itr->second;
//        }
//        return nullptr;
//    }
//};
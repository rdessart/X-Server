#pragma once

#include <UDPServer.h>
#include <Logger.h>
#include "./Managers/DatarefManager.h"
#include "./Managers/FlightLoopManager.h"
#include "./Managers/OperationManager.h"

struct OperationParameters
{
    UDPServer* Server;
    class DatarefManager* DatarefManager;
    class FlightLoopManager* FlightLoopManager;
    class OperationManager* OperationManager;
    Logger* Logger;
};
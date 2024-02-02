#pragma once

#include <UDPServer.h>
#include <Logger.h>

struct OperationParameters
{
    UDPServer* Server;
    class DatarefManager* DatarefManager;
    class FlightLoopManager* FlightLoopManager;
    Logger* Logger;
};
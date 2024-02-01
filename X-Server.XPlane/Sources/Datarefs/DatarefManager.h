#pragma once

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <mutex>

#include <UDPServer.h>
#include <XPLM/XPLMProcessing.h>
#include <XPLM/XPLMPlugin.h>
#include <nlohmann/json.hpp>
#include <Logger.h>
#include <Message.h>

#include "AbstractDataref.h"
#include "FFA320Dataref.h"
#include "Dataref.h"
#include "../Tools/SharedValue.h"

using json = nlohmann::json;

void Callback(double step, void* tag);

struct MasterCallbackParameter {
    UDPServer* Server;
    class DatarefManager* DatarefManager;
};

static void Callback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);

class DatarefManager {
public:
    DatarefManager(bool enableFlightFactorAPI=false);
    AbstractDataref* GetDatarefByName(std::string name);
    void AddDatarefToMap(std::string name, AbstractDataref* dataref);
    void AddMessageToQueue(Message m);

    //void AddCallbackToMap(CallBackInfoStruct info);
    //void* GetCallbackFromMap();
    //int** GetAllCallbacks();

    Message GetNextMessage();
    std::size_t GetMessageQueueLenght();
    Message GetNextMessageOut();
    void AddMessageToOutQueue(Message m);
    std::size_t GetMessageOutQueueLenght();
    std::queue<Message> GetQueue();
    Logger GetLogger();
    bool isFF320Api();
    SharedValuesInterface* GetFF320Interface();

protected:
    Logger m_logger = Logger("X-Server.log", "DatarefManager", false);
    std::map<std::string, AbstractDataref*> m_datarefMap;
    std::map<int, XPLMFlightLoop_f> m_callbackMap;
    //std::queue<
    SharedValuesInterface* m_ff320;
    bool m_isFF320Enable = false;
    std::queue<Message> m_messageQueue;
    std::queue<Message> m_messageOutQueue;
    std::mutex gLock;
};

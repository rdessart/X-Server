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

#include "../Datarefs/AbstractDataref.h"
#include "../Datarefs/FFA320Dataref.h"
#include "../Datarefs/Dataref.h"
#include "../Tools/SharedValue.h"
#include "../MasterCallback.h"

using json = nlohmann::json;
typedef std::pair<std::string, AbstractDataref*> NamedDataref;

class DatarefManager {
/// <summary>
/// This class goal is :
///  - manage dataref operations (setters, getters).
///  - manage dataref storage (register, unregister).
///  - get information about dataref
/// </summary>
public:
    DatarefManager(bool enableFlightFactorAPI=false);
    AbstractDataref* GetDatarefByName(std::string name);
    void BindFlightFactorApiCallback(struct MasterCallbackParameter* parameter);
    void AddDatarefToMap(std::string name, AbstractDataref* dataref);
    Logger GetLogger();
    bool isFF320Api();
    SharedValuesInterface* GetFF320Interface();

protected:
    Logger m_logger = Logger("X-Server.log", "DatarefManager", false);
    std::map<std::string, AbstractDataref*> m_datarefMap;
    std::map<int, XPLMFlightLoop_f> m_callbackMap;
    SharedValuesInterface* m_ff320;
    bool m_isFF320Enable = false;
    std::mutex gLock;
};

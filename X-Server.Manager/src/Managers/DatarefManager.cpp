#include "../../include/Managers/DatarefManager.h"

#include <mutex>
#include <XPLM/XPLMProcessing.h>
#include <XPLM/XPLMPlugin.h>
#include <nlohmann/json.hpp>
#include <Logger.h>
#include <Message.h>


DatarefManager::DatarefManager(bool enableFlightFactorAPI) :
    m_isFF320Enable(false), m_ff320(0)
{
    if(enableFlightFactorAPI)
    {
        m_ff320 = new SharedValuesInterface();
        m_logger.Log("Initalising FF320 Data Interface...");
        int ffPluginID = XPLMFindPluginBySignature(XPLM_FF_SIGNATURE);
        if(ffPluginID == XPLM_NO_PLUGIN_ID)
        {
            m_logger.Log("Plugin not found !", Logger::Severity::CRITICAL);
            return;
        }
        m_logger.Log("FF320 plugin ID : " + std::to_string(ffPluginID));
        XPLMSendMessageToPlugin(ffPluginID, XPLM_FF_MSG_GET_SHARED_INTERFACE, m_ff320);
        if (m_ff320->DataVersion == NULL) {
            m_logger.Log("[FF320API] Unable to load FlightFactorA320 API!");
            return;
        }
        unsigned int ffAPIdataversion = m_ff320->DataVersion();
        m_logger.Log("[FF320API] Version : " + std::to_string(ffAPIdataversion));
        m_isFF320Enable = true;
    }

}

void DatarefManager::BindFlightFactorApiCallback(OperationParameters* parameter)
{
    if (m_isFF320Enable)
        m_ff320->DataAddUpdate(Callback, parameter);
}

AbstractDataref *DatarefManager::GetDatarefByName(std::string name)
{
    AbstractDataref* dataref = nullptr;
    gLock.lock();
        if(!m_datarefMap.contains(name))
        {
            m_logger.Log("dataref nammed: '" + name + "' NOT FOUND!");
        }
        else 
        {
            dataref = m_datarefMap.at(name);
        }
    gLock.unlock();
    return dataref;
}

void DatarefManager::AddDatarefToMap(std::string name, AbstractDataref* dataref)
{
    gLock.lock();
        if(m_datarefMap.contains(name))
        {
            auto d = m_datarefMap.find(name);
            m_datarefMap.erase(d);
        }
        m_datarefMap.emplace(name, dataref);
    gLock.unlock();
}

Logger DatarefManager::GetLogger()
{
    return m_logger;
}

bool DatarefManager::isFF320Api()
{
    return m_isFF320Enable;
}

SharedValuesInterface* DatarefManager::GetFF320Interface()
{
    return m_ff320;
}

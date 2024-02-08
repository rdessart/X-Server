#include "Operations.h"
#include "DatarefManager.h"

#include <XPLM/XPLMUtilities.h>
#include <Datarefs/AbstractDataref.h>
#include <Datarefs/Dataref.h>
#include <Datarefs/FFA320Dataref.h>

#define NAMEOF(name) #name

OPERATION_API int InitializeDLL(Manager* manager)
{
    return static_cast<int>(manager->AddService(NAMEOF(DatarefManager), new DatarefManager(true)));
}

OPERATION_API int UninitializeDLL(Manager* manager)
{
    DatarefManager* service = static_cast<DatarefManager*>(manager->GetService("DatarefManager"));
    manager->RemoveService(NAMEOF(DatarefManager));
    delete service;
    return EXIT_SUCCESS;
}

OPERATION_API int GetOperations(std::map<std::string, std::string>* operationsNames)
{
    if (operationsNames == nullptr) return -1;
    size_t sizeBefore = operationsNames->size();
    operationsNames->emplace("speak",          NAMEOF(SpeakOperation));
    operationsNames->emplace("setdata",        NAMEOF(SetDatarefOperation));
    operationsNames->emplace("getdata",        NAMEOF(GetDatarefOperation));
    operationsNames->emplace("regdata",        NAMEOF(RegisterDatarefOperation));
    operationsNames->emplace("setregdata",     NAMEOF(SetRegisteredDatarefOperation));
    operationsNames->emplace("getregdata",     NAMEOF(GetRegisteredDatarefOperation));
    operationsNames->emplace("datainfo",       NAMEOF(GetDatarefInfoOperation));
    operationsNames->emplace("regdatainfo",    NAMEOF(GetRegisteredDatarefInfoOperation));

    return (int)(operationsNames->size() - sizeBefore);
}

OPERATION_API void SpeakOperation(Message& m, Manager* manager)
{
    XPLMSpeakString(m.message.value("Text", "").c_str());
    m.message["Result"] = "Ok";
}

OPERATION_API void SetDatarefOperation(Message& m, Manager* manager)
{
    manager->GetLogger()->Log("Setting dataref", "X-Server.Operations.DLL");
    if (!m.message.contains("Dataref")) 
    { 
        m.message["Result"] = "Error:Missing Dataref Section";
        return; 
    }
    DatarefManager* datarefManager = static_cast<DatarefManager*>(manager->GetService(NAMEOF(DatarefManager)));
    AbstractDataref* d;
    std::string link = m.message["Dataref"]["Link"].get<std::string>();


    if (datarefManager->isFF320Api() && link.find("Aircraft") != std::string::npos && link.find(".") != std::string::npos)
        d = new FFDataref(datarefManager->GetFF320Interface());
    else
        d = new Dataref();

    d->FromJson(m.message["Dataref"]);
    m.message["Result"] = "Ok";
    free(d);
}

OPERATION_API void GetDatarefOperation(Message& m, Manager* manager)
{
    manager->GetLogger()->Log("Getting dataref", "X-Server.Operations.DLL");
    if (!m.message.contains("Dataref"))
    {
        m.message["Result"] = "Error:Missing Dataref entry in JSON";
        return;
    }
    DatarefManager* datarefManager = static_cast<DatarefManager*>(manager->GetService(NAMEOF(DatarefManager)));
    std::string link = m.message["Dataref"]["Link"].get<std::string>();
    AbstractDataref* d;
    if (datarefManager->isFF320Api() &&
        link.find("Aircraft") != std::string::npos &&
        link.find(".") != std::string::npos)
    {
        d = new FFDataref(datarefManager->GetFF320Interface());
    }
    else {
        d = new Dataref();
    }
    d->FromJson(m.message["Dataref"]);
    m.message["Dataref"]["Value"] = d->GetValue();
    m.message["Result"] = "Ok";
    free(d);
}

OPERATION_API void RegisterDatarefOperation(Message& m, Manager* manager)
{
    manager->GetLogger()->Log("Registering dataref", "X-Server.Operations.DLL");
    if (!m.message.contains("Dataref")) {
        m.message["Result"] = "Error:Missing Dataref entry in JSON";
        return;
    }
    if (!m.message.contains("Name"))
    {
        m.message["Result"] = "Error:Missing 'Name' field";
        return;
    }
    DatarefManager* datarefManager = static_cast<DatarefManager*>(manager->GetService(NAMEOF(DatarefManager)));
    std::string link = m.message["Dataref"]["Link"].get<std::string>();
    std::string name = m.message["Name"].get<std::string>();
    manager->GetLogger()->Log("Adding : '" + name + std::string("'"), "X-Server.Operations.DLL");
    AbstractDataref* d;
    if (datarefManager->isFF320Api() &&
        link.find("Aircraft") != std::string::npos &&
        link.find(".") != std::string::npos)
    {
        d = new FFDataref(datarefManager->GetFF320Interface());
    }
    else {
        d = new Dataref();
    }
    d->FromJson(m.message["Dataref"]);
    datarefManager->AddDatarefToMap(name, d);
    m.message["Result"] = "Ok";
}

OPERATION_API void SetRegisteredDatarefOperation(Message& m, Manager* manager)
{
    manager->GetLogger()->Log("Setting registered dataref", "X-Server.Operations.DLL");
    if (!m.message.contains("Name"))
    {
        m.message["Result"] = "Error:Missing 'Name' field";
        return;
    }
    if (!m.message.contains("Value"))
    {
        m.message["Result"] = "Error:Missing 'Value' field";
        return;
    }
    DatarefManager* datarefManager = static_cast<DatarefManager*>(manager->GetService(NAMEOF(DatarefManager)));
    if (m.message["Name"].type() == json::value_t::object)
    {
        auto values = json::array();
        for (auto it = m.message["Name"].begin(); it != m.message["Name"].end(); ++it)
        {
            AbstractDataref* d = datarefManager->GetDatarefByName(it.key());
            if (d == nullptr) {
                m.message["Result"] = "Error:Dataref not in map !";
                return;
            }
            d->SetValue(it.value());
            values.push_back(d->GetValue());
        }
        m.message["Value"] = values;
    }
    else if (m.message["Name"].type() == json::value_t::array && m.message["Value"].type() != json::value_t::array)
    {
        m.message["Result"] = "Error:Name is an array but value is a single element";
        return;
    }
    else {
        AbstractDataref* d = datarefManager->GetDatarefByName(m.message.value("Name", ""));
        if (d == nullptr) {
            m.message["Result"] = "Error:Dataref not in map !";
            return;
        }
        d->SetValue(m.message.value("Value", ""));
        m.message["Value"] = d->GetValue();
    }
    m.message["Result"] = "Ok";
}

OPERATION_API void GetRegisteredDatarefOperation(Message& m, Manager* manager)
{
    manager->GetLogger()->Log("Setting registered dataref");
    if (!m.message.contains("Name"))
    {
        m.message["Result"] = "Error:Missing 'Name' field";
        return;
    }
    DatarefManager* datarefManager = static_cast<DatarefManager*>(manager->GetService(NAMEOF(DatarefManager)));
    if (m.message["Name"].type() == json::value_t::array)
    {
        m.message["Value"] = json();
        for (json& it : m.message["Name"])
        {
            std::string name = it.get<std::string>();
            AbstractDataref* d = datarefManager->GetDatarefByName(name);
            if (d == nullptr) {
                m.message["Result"] = "Error:Dataref not in map !";
                return;
            }
            m.message["Value"].push_back({ name, d->GetValue() });
        }
        m.message.erase("Name");
    }
    else {
        AbstractDataref* d = datarefManager->GetDatarefByName(m.message.value("Name", ""));
        if (d == nullptr) {
            m.message["Result"] = "Error:Dataref not in map !";
            return;;
        }
        m.message["Value"] = d->GetValue();
    }
    m.message["Result"] = "Ok";
}

OPERATION_API void GetDatarefInfoOperation(Message& m, Manager* manager)
{
    manager->GetLogger()->Log("Getting dataref Info", "X-Server.Operations.DLL");
    if (!m.message.contains("Dataref")) {
        m.message["Result"] = "Error:Missing Dataref entry in JSON";
        return;
    }
    DatarefManager* datarefManager = static_cast<DatarefManager*>(manager->GetService(NAMEOF(DatarefManager)));
    std::string link = m.message["Dataref"]["Link"].get<std::string>();
    AbstractDataref* d;
    if (datarefManager->isFF320Api() &&
        link.find("Aircraft") != std::string::npos &&
        link.find(".") != std::string::npos)
    {
        d = new FFDataref(datarefManager->GetFF320Interface());
    }
    else {
        d = new Dataref();
    }
    d->FromJson(m.message["Dataref"]);
    m.message["Dataref"] = d->ToJson();
}

OPERATION_API void GetRegisteredDatarefInfoOperation(Message& m, Manager* manager)
{
    manager->GetLogger()->Log("Getting registered dataref Info", "X-Server.Operations.DLL");
    if (!m.message.contains("Name"))
    {
        m.message["Result"] = "Error:Missing 'Name' field";
        return;
    }
    DatarefManager* datarefManager = static_cast<DatarefManager*>(manager->GetService(NAMEOF(DatarefManager)));
    AbstractDataref* d = datarefManager->GetDatarefByName(m.message.value("Name", ""));
    if (d == nullptr) {
        m.message["Result"] = "Error:Dataref not in map !";
        return;
    }

    m.message["Dataref"] = d->ToJson();
    m.message["Result"] = "Ok";
}
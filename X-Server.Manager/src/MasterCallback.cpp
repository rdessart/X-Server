#include "../include/MasterCallback.h"

void Callback(double step, void* tag)
{
    OperationParameters* parameters = (OperationParameters*)tag;
    Message reveivedMessage;
    while (parameters->Server->GetWaitingMessage(reveivedMessage))
    {
        parameters->Logger->Log("Message : '" + reveivedMessage.message.dump() + "'");
        if (!reveivedMessage.message.contains("Operation")) continue;
        std::string operationName = reveivedMessage.message.value("Operation", "");
        OperationPointer ops = CallbackOperations::GetOperation(operationName);
        if (ops == nullptr)
        {
            parameters->Logger->Log("Operation NOT found : '" + operationName + "'!", Logger::Severity::WARNING);
            return;
        }
        ops(reveivedMessage, parameters);
        parameters->Server->SendData(reveivedMessage);
    }
}

void SpeakOperation(Message& m, OperationParameters* parameters)
{
    XPLMSpeakString(m.message.value("Text", "").c_str());
    m.message["Result"] = "Ok";
}

void SetDatarefOperation(Message& m, OperationParameters* parameters)
{
    parameters->Logger->Log("Setting dataref");
    if (!m.message.contains("Dataref")) return;
    AbstractDataref* d;
    std::string link = m.message["Dataref"]["Link"].get<std::string>();

    if (parameters->DatarefManager->isFF320Api() && link.find("Aircraft") != std::string::npos && link.find(".") != std::string::npos)
        d = new FFDataref(parameters->DatarefManager->GetFF320Interface());
    else
        d = new Dataref();

    d->FromJson(m.message["Dataref"]);
    m.message["Result"] = "Ok";
    free(d);
}

void GetDatarefOperation(Message& m, OperationParameters* parameters)
{
    parameters->Logger->Log("Getting dataref");
    if (!m.message.contains("Dataref"))
    {
        m.message["Result"] = "Error:Missing Dataref entry in JSON";
        return;
    }
    std::string link = m.message["Dataref"]["Link"].get<std::string>();
    AbstractDataref* d;
    if (parameters->DatarefManager->isFF320Api() &&
        link.find("Aircraft") != std::string::npos &&
        link.find(".") != std::string::npos)
    {
        d = new FFDataref(parameters->DatarefManager->GetFF320Interface());
    }
    else {
        d = new Dataref();
    }
    d->FromJson(m.message["Dataref"]);
    m.message["Dataref"]["Value"] = d->GetValue();
    m.message["Result"] = "Ok";
    free(d);
}

void RegisterDatarefOperation(Message& m, OperationParameters* parameters)
{
    parameters->Logger->Log("Registering dataref");
    if (!m.message.contains("Dataref")) {
        m.message["Result"] = "Error:Missing Dataref entry in JSON";
        return;
    }
    if (!m.message.contains("Name"))
    {
        m.message["Result"] = "Error:Missing 'Name' field";
        return;
    }
    std::string link = m.message["Dataref"]["Link"].get<std::string>();
    std::string name = m.message["Name"].get<std::string>();
    parameters->Logger->Log("Adding : '" + name + std::string("'"));
    AbstractDataref* d;
    if (parameters->DatarefManager->isFF320Api() &&
        link.find("Aircraft") != std::string::npos &&
        link.find(".") != std::string::npos)
    {
        d = new FFDataref(parameters->DatarefManager->GetFF320Interface());
    }
    else {
        d = new Dataref();
    }
    d->FromJson(m.message["Dataref"]);
    parameters->DatarefManager->AddDatarefToMap(name, d);
    m.message["Result"] = "Ok";
}

void SetRegisteredDatarefOperation(Message& m, OperationParameters* parameters)
{
    parameters->Logger->Log("Setting registered dataref");
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
    if (m.message["Name"].type() == json::value_t::object)
    {
        auto values = json::array();
        for (auto it = m.message["Name"].begin(); it != m.message["Name"].end(); ++it)
        {
            AbstractDataref* d = parameters->DatarefManager->GetDatarefByName(it.key());
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
        AbstractDataref* d = parameters->DatarefManager->GetDatarefByName(m.message.value("Name", ""));
        if (d == nullptr) {
            m.message["Result"] = "Error:Dataref not in map !";
            return;
        }
        d->SetValue(m.message.value("Value", ""));
        m.message["Value"] = d->GetValue();
    }
    m.message["Result"] = "Ok";
}

void GetRegisteredDatarefOperation(Message& m, OperationParameters* parameters)
{
    parameters->Logger->Log("Setting registered dataref");
    if (!m.message.contains("Name"))
    {
        m.message["Result"] = "Error:Missing 'Name' field";
        return;
    }
    if (m.message["Name"].type() == json::value_t::array)
    {
        m.message["Value"] = json();
        for (json& it : m.message["Name"])
        {
            std::string name = it.get<std::string>();
            AbstractDataref* d = parameters->DatarefManager->GetDatarefByName(name);
            if (d == nullptr) {
                m.message["Result"] = "Error:Dataref not in map !";
                return;
            }
            m.message["Value"].push_back({ name, d->GetValue() });
        }
        m.message.erase("Name");
    }
    else {
        AbstractDataref* d = parameters->DatarefManager->GetDatarefByName(m.message.value("Name", ""));
        if (d == nullptr) {
            m.message["Result"] = "Error:Dataref not in map !";
            return;;
        }
        m.message["Value"] = d->GetValue();
    }
    m.message["Result"] = "Ok";
}

void GetDatarefInfoOperation(Message& m, OperationParameters* parameters)
{
    parameters->Logger->Log("Getting dataref");
    if (!m.message.contains("Dataref")) {
        m.message["Result"] = "Error:Missing Dataref entry in JSON";
        return;
    }
    std::string link = m.message["Dataref"]["Link"].get<std::string>();
    AbstractDataref* d;
    if (parameters->DatarefManager->isFF320Api() &&
        link.find("Aircraft") != std::string::npos &&
        link.find(".") != std::string::npos)
    {
        d = new FFDataref(parameters->DatarefManager->GetFF320Interface());
    }
    else {
        d = new Dataref();
    }
    d->FromJson(m.message["Dataref"]);
    m.message["Dataref"] = d->ToJson();
}

void GetRegisteredDatarefInfoOperation(Message& m, OperationParameters* parameters)
{
    parameters->Logger->Log("Setting registered dataref");
    if (!m.message.contains("Name"))
    {
        m.message["Result"] = "Error:Missing 'Name' field";
        return;
    }

    AbstractDataref* d = parameters->DatarefManager->GetDatarefByName(m.message.value("Name", ""));
    if (d == nullptr) {
        m.message["Result"] = "Error:Dataref not in map !";
        return;
    }

    m.message["Dataref"] = d->ToJson();
    m.message["Result"] = "Ok";
}

void RegisterFlightLoopOperation(Message& m, OperationParameters* parameters) 
/// <summary>
/// Required Fields:
///     json : CallbackInfo {
///         INT  : DeltaTime,
///         BOOL : IsTime 
///     }
/// </summary>
{
    if (!m.message.contains("CallbackInfo"))
    {
        m.message["Result"] = "Error:Missing Required CallbackInfo Section";
        return;
    }
    unsigned int deltatime = m.message["CallbackInfo"]["DeltaTime"].get<unsigned int>();
    bool timeRelative = m.message["CallbackInfo"]["IsTime"].get<bool>();
    unsigned int flightloopId = 0;

    if (!parameters->FlightLoopManager->FlightLoopExist(deltatime, timeRelative))
    {
        Message m2;
        m2.target = m.target;
        m2.target_lenght = m.target_lenght;
        flightloopId = parameters->FlightLoopManager->GetFlightLoop(deltatime, timeRelative, parameters, m2);
    }
    else
        flightloopId = parameters->FlightLoopManager->GetFlightLoop(deltatime, timeRelative, nullptr, m);
    m.message["Result"] = "Ok";
    m.message["CallbackId"] = flightloopId;
}

void SubscribeDatarefOperation(Message& m, OperationParameters* parameters)
{
    if (!m.message.contains("CallbackId"))
    {
        m.message["Result"] = "Error:Missing Required CallbackId";
        return;
    }

    if (!m.message.contains("Name"))
    {
        m.message["Result"] = "Error:Missing Required Dataref Name";
        return;
    }
    std::string datarefName = m.message["Name"].get<std::string>();
    AbstractDataref* dataref = parameters->DatarefManager->GetDatarefByName(datarefName);
    unsigned int callbackId = m.message["CallbackId"].get<unsigned int>();
    if (dataref == nullptr)
    {
        m.message["Result"] = "Error:Dataref was not registered before call";
        return;
    }
    if (!parameters->FlightLoopManager->FlightLoopExist(callbackId))
    {
        m.message["Result"] = "Error:callback not found";
        return;
    }

    parameters->FlightLoopManager->AssignDatarefToFlightLoop(callbackId, datarefName, dataref);
    m.message["Result"] = "Ok";
    return;
}
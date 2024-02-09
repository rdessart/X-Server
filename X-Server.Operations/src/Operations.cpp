#include "Operations.h"
#include "DatarefManager.h"

#include <XPLM/XPLMUtilities.h>
#include <Datarefs/AbstractDataref.h>
#include <Datarefs/Dataref.h>
#include <Datarefs/FFA320Dataref.h>
#include <XPLM/XPLMPlanes.h>
#include <XPLM/XPLMGraphics.h>

#define NAMEOF(name) #name

OPERATION_API int InitializeDLL(Manager* manager)
{
    manager->AddService(NAMEOF(FlightLoopManager), new FlightLoopManager());
    manager->AddService(NAMEOF(DatarefManager), new DatarefManager(true));
    return 1;
}

OPERATION_API int UninitializeDLL(Manager* manager)
{
    DatarefManager* datarefService = static_cast<DatarefManager*>(manager->GetService(NAMEOF(DatarefManager)));
    delete datarefService;
    manager->RemoveService(NAMEOF(DatarefManager));

    FlightLoopManager* flightloopService = static_cast<FlightLoopManager*>(manager->GetService(NAMEOF(FlightLoopManager)));
    delete flightloopService;
    manager->RemoveService(NAMEOF(FlightLoopManager));

    return EXIT_SUCCESS;
}

OPERATION_API int GetOperations(std::map<std::string, std::string>* operationsNames)
{
    if (operationsNames == nullptr) return -1;
    size_t sizeBefore = operationsNames->size();
    operationsNames->emplace("speak",           NAMEOF(SpeakOperation));
    operationsNames->emplace("setdata",         NAMEOF(SetDatarefOperation));
    operationsNames->emplace("getdata",         NAMEOF(GetDatarefOperation));
    operationsNames->emplace("regdata",         NAMEOF(RegisterDatarefOperation));
    operationsNames->emplace("setregdata",      NAMEOF(SetRegisteredDatarefOperation));
    operationsNames->emplace("getregdata",      NAMEOF(GetRegisteredDatarefOperation));
    operationsNames->emplace("datainfo",        NAMEOF(GetDatarefInfoOperation));
    operationsNames->emplace("regdatainfo",     NAMEOF(GetRegisteredDatarefInfoOperation));
    operationsNames->emplace("regflightloop",   NAMEOF(RegisterFlightLoopOperation));
    operationsNames->emplace("subdata",         NAMEOF(SubscribeDatarefOperation));
    operationsNames->emplace("unsubdata",       NAMEOF(UnsubscribeDatarefOperation));
    operationsNames->emplace("unregflightloop", NAMEOF(UnregisterFlightLoopOperation));
    operationsNames->emplace("aquireplanes",    NAMEOF(AquirePlanes));
    operationsNames->emplace("releaseplanes",   NAMEOF(ReleasePlanes));
    operationsNames->emplace("setplanecount",   NAMEOF(SetPlanesCount));
    operationsNames->emplace("updateplanes",   NAMEOF(UpdatePlanes));
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

OPERATION_API void RegisterFlightLoopOperation(Message& m, Manager* manager)
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
    intptr_t flightloopId = 0;
    FlightLoopManager* flm = static_cast<FlightLoopManager*>(manager->GetService(NAMEOF(FlightLoopManager)));
    if (!flm->FlightLoopExist(deltatime, timeRelative))
    {
        Message m2;
        m2.target = m.target;
        m2.target_lenght = m.target_lenght;
        flightloopId = (intptr_t)(flm->GetFlightLoop(deltatime, timeRelative, manager, m2));
    }
    else
        flightloopId = (intptr_t)(flm->GetFlightLoop(deltatime, timeRelative, nullptr, m));
    m.message["Result"] = "Ok";
    m.message["CallbackId"] = flightloopId;
}

OPERATION_API void SubscribeDatarefOperation(Message& m, Manager* manager)
{
    if (!m.message.contains("CallbackId"))
    {
        m.message["Result"] = "Error:Missing Required CallbackId";
        return;
    }

    if (!m.message.contains("Dataref"))
    {
        m.message["Result"] = "Error:Missing Required Dataref Section";
        return;
    }

    if (!m.message.contains("Name"))
    {
        m.message["Result"] = "Error:Missing Required Dataref Name";
        return;
    }

    std::string datarefName = m.message["Name"].get<std::string>();
    AbstractDataref* dataref = new Dataref();
    dataref->FromJson(m.message["Dataref"]);
    FlightLoopManager* flm = static_cast<FlightLoopManager*>(manager->GetService(NAMEOF(FlightLoopManager)));
    XPLMFlightLoopID callbackId = (XPLMFlightLoopID)m.message["CallbackId"].get<intptr_t>();

    if (dataref == nullptr)
    {
        m.message["Result"] = "Error:Dataref was not registered before call";
        return;
    }
    if (!flm->FlightLoopExist(callbackId))
    {
        m.message["Result"] = "Error:callback not found";
        return;
    }

    flm->AssignDatarefToFlightLoop(callbackId, datarefName, dataref);
    m.message["Result"] = "Ok";
    return;
}

OPERATION_API void UnsubscribeDatarefOperation(Message& m, Manager* manager)
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
    XPLMFlightLoopID callbackId = (XPLMFlightLoopID)m.message["CallbackId"].get<intptr_t>();
    FlightLoopManager* flm = static_cast<FlightLoopManager*>(manager->GetService(NAMEOF(FlightLoopManager)));
    if (!flm->UnassignDatarefToFlightLoop(callbackId, datarefName))
    {
        m.message["Result"] = "Error:Unexceptected event while unregistering dataref";
        return;
    }
    m.message["Result"] = "Ok";
}

OPERATION_API void UnregisterFlightLoopOperation(Message& m, Manager* manager)
{
    if (!m.message.contains("CallbackId"))
    {
        m.message["Result"] = "Error:Missing Required CallbackId";
        return;
    }

    XPLMFlightLoopID callbackId = (XPLMFlightLoopID)m.message["CallbackId"].get<intptr_t>();
    FlightLoopManager* flm = static_cast<FlightLoopManager*>(manager->GetService(NAMEOF(FlightLoopManager)));
    if (!flm->DeleteFlightLoop(callbackId))
    {
        m.message["Result"] = "Error:Unexceptected event while unregistering dataref";
        return;
    }
    m.message["Result"] = "Ok";
}

OPERATION_API void AquirePlanes(Message& message, Manager* manager)
{
    //TODO: check if we can pass a callback to be called when we have plane acess.
    int res = XPLMAcquirePlanes(nullptr, nullptr, nullptr);
    if (!res)
    {
        message.message["Result"] = "Error:Unable to get plane aquisition()";
        return;
    }
    Dataref dataref;
    dataref.Load("sim/operation/override/override_TCAS");
    dataref.SetValue(1);
    message.message["Result"] = "Ok";
    //registering dataref
    DatarefManager* datarefManager = static_cast<DatarefManager*>(manager->GetService(NAMEOF(DatarefManager)));
    datarefManager->AddDatarefToMap("TCAS_IDENT_S"   , "sim/cockpit2/tcas/targets/modeS_id");
    datarefManager->AddDatarefToMap("TCAS_CODE_C"    , "sim/cockpit2/tcas/targets/modeC_code");
    datarefManager->AddDatarefToMap("TCAS_FLIGHT_ID" , "sim/cockpit2/tcas/targets/flight_id");
    datarefManager->AddDatarefToMap("TCAS_ICAO_TYPES", "sim/cockpit2/tcas/targets/icao_type");
    datarefManager->AddDatarefToMap("TCAS_X"         , "sim/cockpit2/tcas/targets/position/x");
    datarefManager->AddDatarefToMap("TCAS_Y"         , "sim/cockpit2/tcas/targets/position/y");
    datarefManager->AddDatarefToMap("TCAS_Z"         , "sim/cockpit2/tcas/targets/position/z");
    datarefManager->AddDatarefToMap("TCAS_PITCH"     , "sim/cockpit2/tcas/targets/position/the");
    datarefManager->AddDatarefToMap("TCAS_ROLL"      , "sim/cockpit2/tcas/targets/position/phi");
    datarefManager->AddDatarefToMap("TCAS_HDG"       , "sim/cockpit2/tcas/targets/position/psi");
    //XP12 AND +
    if (manager->GetSDKVersion() >= 400)
    {
        datarefManager->AddDatarefToMap("TCAS_WING_SPAN", "sim/cockpit2/tcas/targets/wake/wing_span_m");
        datarefManager->AddDatarefToMap("TCAS_WING_AREA", "sim/cockpit2/tcas/targets/wake/wing_area_m2");
        datarefManager->AddDatarefToMap("TCAS_WAKE_CAT", "sim/cockpit2/tcas/targets/wake/wake_cat");
        datarefManager->AddDatarefToMap("TCAS_MASS", "sim/cockpit2/tcas/targets/wake/mass_kg");
        datarefManager->AddDatarefToMap("TCAS_AOA", "sim/cockpit2/tcas/targets/wake/aoa");
        datarefManager->AddDatarefToMap("TCAS_LIFT", "sim/cockpit2/tcas/targets/wake/lift_N");
    }
    //Multiplayers : 
    for (int i(1); i <= 19; i++)
    {
        datarefManager->AddDatarefToMap("PLAYER_" + std::to_string(i) + "_X", "sim/multiplayer/position/plane" + std::to_string(i) + "_x");
        datarefManager->AddDatarefToMap("PLAYER_" + std::to_string(i) + "_Y", "sim/multiplayer/position/plane" + std::to_string(i) + "_y");
        datarefManager->AddDatarefToMap("PLAYER_" + std::to_string(i) + "_Z", "sim/multiplayer/position/plane" + std::to_string(i) + "_z");
        datarefManager->AddDatarefToMap("PLAYER_" + std::to_string(i) + "_PITCH", "sim/multiplayer/position/plane" + std::to_string(i) + "_the");
        datarefManager->AddDatarefToMap("PLAYER_" + std::to_string(i) + "_ROLL", "sim/multiplayer/position/plane" + std::to_string(i) + "_phi");
        datarefManager->AddDatarefToMap("PLAYER_" + std::to_string(i) + "_HDG", "sim/multiplayer/position/plane" + std::to_string(i) + "_psi");
    }

}

OPERATION_API void ReleasePlanes(Message& message, Manager* manager)
{
    XPLMReleasePlanes();
    message.message["Result"] = "Ok";
}

OPERATION_API void SetPlanesCount(Message& message, Manager* manager)
{
    int count = message.message["Count"].get<int>();
    XPLMSetActiveAircraftCount(count);
    message.message["Result"] = "Ok";
}

OPERATION_API void UpdatePlanes(Message& message, Manager* manager)
/*
*     planes : {
        "ModeS" : [0xFF_FF_FF],
        "ModeC" : [1234],
        "FlightId" : ["BEL123"],
        "IcaoType" : ["A320"],
        "Position" : [{
            "Latitude" : 50.0,
            "Longitude": 5.0,
            "Elevation" : 100.0,
        }],
        "WingSpan" : [35.80],
        "WingArea" : [122.6],
        "WakeCat" : [2],
        "Mass" : [62000],
        "AOA" : [5.5],
        "Lift" : [431027.13011943013] #62000 * 9.81 * math.cos(5.5)
    }
*/
{
    json j = message.message["Planes"];
    DatarefManager* datarefManager = static_cast<DatarefManager*>(manager->GetService(NAMEOF(DatarefManager)));

    try {
        datarefManager->GetDatarefByName("TCAS_IDENT_S")->SetValue(j["ModeS"], 1);
        datarefManager->GetDatarefByName("TCAS_CODE_C")->SetValue(j["ModeC"], 1);
        //XP12 AND +
        if (manager->GetSDKVersion() >= 400)
        {
            datarefManager->GetDatarefByName("TCAS_WING_SPAN")->SetValue(j["WingSpan"], 1);
            datarefManager->GetDatarefByName("TCAS_WING_AREA")->SetValue(j["WingArea"], 1);
            datarefManager->GetDatarefByName("TCAS_WAKE_CAT")->SetValue(j["WakeCat"], 1);
            datarefManager->GetDatarefByName("TCAS_MASS")->SetValue(j["Mass"], 1);
            datarefManager->GetDatarefByName("TCAS_AOA")->SetValue(j["AOA"], 1);
            datarefManager->GetDatarefByName("TCAS_LIFT")->SetValue(j["Lift"], 1);
        }
    }
    catch(...)
    {
        message.message["Result"] = "Error:Dataref weren't loaded did you ask to aquire the plane before this call ?";
        return;
    }

   /* datarefManager->GetDatarefByName("TCAS_CODE_C")->SetValue(j["ModeC"]);
    std::vector<std::string> flightsId = j["FlightId"].get<std::vector<std::string>>();
    std::stringstream flightsIdStream;
    for (auto& id : flightsId)
    {
        flightsIdStream << id << "\0";
    }
    datarefManager->GetDatarefByName("TCAS_FLIGHT_ID" )->SetValue(flightsIdStream.str());

    std::vector<std::string> icaoTypes = j["IcaoType"].get<std::vector<std::string>>();
    std::stringstream icaoTypesStream;
    for (auto& id : icaoTypes)
    {
        icaoTypesStream << id << "\0";
    }
    datarefManager->GetDatarefByName("TCAS_ICAO_TYPES")->SetValue(icaoTypesStream.str());*/

    json positions = j["Positions"];
    json tcasX = json::array();
    json tcasY = json::array();
    json tcasZ = json::array();
    json tcasPitch = json::array();
    json tcasHdg = json::array();
    json tcasRoll = json::array();
    int max = 19;
    int i = 1;
    for (json::iterator it = positions.begin(); it != positions.end(); it++)
    {
        double latitude  = (*it)["Latitude" ].get<double>();
        double longitude = (*it)["Longitude"].get<double>();
        double elevation = (*it)["Elevation"].get<double>();
        double pitch     = (*it)["Pitch"].get<double>();
        double hdg       = (*it)["Heading"].get<double>();
        double roll      = (*it)["Roll"].get<double>();
        double oX(0.0), oY(0.0), oZ(0.0);
        XPLMWorldToLocal(latitude, longitude, elevation, &oX, &oY, &oZ);
        tcasX.push_back(oX);
        tcasY.push_back(oY);
        tcasZ.push_back(oZ);
        tcasPitch.push_back(pitch);
        tcasHdg.push_back(hdg);
        tcasRoll.push_back(roll);
        if (i < max)
        {
            std::string name = "PLAYER_" + std::to_string(i) + "_X";
            try {
                datarefManager->GetDatarefByName("PLAYER_" + std::to_string(i) + "_X")->SetValue(oX);
                datarefManager->GetDatarefByName("PLAYER_" + std::to_string(i) + "_Y")->SetValue(oY);
                datarefManager->GetDatarefByName("PLAYER_" + std::to_string(i) + "_Z")->SetValue(oZ);
                datarefManager->GetDatarefByName("PLAYER_" + std::to_string(i) + "_PITCH")->SetValue(pitch);
                datarefManager->GetDatarefByName("PLAYER_" + std::to_string(i) + "_ROLL")->SetValue(hdg);
                datarefManager->GetDatarefByName("PLAYER_" + std::to_string(i) + "_HDG")->SetValue(roll);
            }
            catch (...)
            {
                continue;
            }
        }
        i++;
    }

    datarefManager->GetDatarefByName("TCAS_X")->SetValue(tcasX, 1);
    datarefManager->GetDatarefByName("TCAS_Y")->SetValue(tcasY, 1);
    datarefManager->GetDatarefByName("TCAS_Z")->SetValue(tcasZ, 1);
    datarefManager->GetDatarefByName("TCAS_PITCH")->SetValue(tcasPitch, 1);
    datarefManager->GetDatarefByName("TCAS_ROLL")->SetValue(tcasHdg, 1);
    datarefManager->GetDatarefByName("TCAS_HDG")->SetValue(tcasRoll, 1);


    return;
}
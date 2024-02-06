from udp_client_service import UDPClientService
import time

CALLBACK_ID = 2210060538704

def load_operations_dll(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "loaddll","Path" : "XServer.Operations.dll"})

def unload_operations_dll(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "unloaddll","Path" : "XServer.Operations.dll"})

def speak(client: UDPClientService) -> None:
    client.sendMessage({"Operation" : "Speak", "Text" : "Hello Python"})

def set_smoke_on(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "SetData","Dataref" : {"Link" : "sim/operation/failures/rel_smoke_cpit","Value" : "6"}})

def set_smoke_off(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "SetData","Dataref" : {"Link" : "sim/operation/failures/rel_smoke_cpit","Value" : "0"}})

def get_smoke_value(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "GetData","Dataref" : { "Link" : "sim/operation/failures/rel_smoke_cpit"}})

def register_dataref(client: UDPClientService) -> None:
     client.sendMessage({"Operation": "regdata","Name" : "SMOKE", "Dataref" : {"Link" : "sim/operation/failures/rel_smoke_cpit"}})

def set_register_dataref_on(client: UDPClientService) -> None:
     client.sendMessage({ "Operation": "setregdata","Name" : "SMOKE","Value" : "6"})

def set_register_dataref_off(client: UDPClientService) -> None:
     client.sendMessage({ "Operation": "setregdata","Name" : "SMOKE","Value" : "0"})

def load_flightloop_dll(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "loaddll","Path" : "X-Server.DevOperations.dll"})

def unload_flightloop_dll(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "unloaddll","Path" : "X-Server.DevOperations.dll"})

def speak2(client: UDPClientService) -> None:
     client.sendMessage({"Operation": "Speak2"})

def register_flightloop(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "regflightloop", "CallbackInfo" : { "DeltaTime" : 1000, "IsTime": True}})

def subscribe_dataref(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "subdata","Name": "ALTITUDE_FT", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/flightmodel/position/elevation", "Type": "DOUBLE", "ConversionFactor" : "3.28084"}})
    time.sleep(0.1)
    client.sendMessage({"Operation": "subdata","Name": "ALTITUDE_M", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/flightmodel/position/elevation", "Type": "DOUBLE"}})
    time.sleep(0.1)
    client.sendMessage({"Operation": "subdata","Name": "IAS", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/flightmodel/position/indicated_airspeed", "Type": "FLOAT"}})
    time.sleep(0.1)
    client.sendMessage({"Operation": "subdata","Name": "GS", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/flightmodel/position/groundspeed", "Type": "FLOAT",  "ConversionFactor" : "1.94384"}})
    time.sleep(0.1)
    client.sendMessage({"Operation": "subdata","Name": "LATITUDE", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/flightmodel/position/latitude", "Type": "DOUBLE"}})
    time.sleep(0.1)
    client.sendMessage({"Operation": "subdata","Name": "LONGITUDE", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/flightmodel/position/longitude", "Type": "DOUBLE"}})
    time.sleep(0.1)
    client.sendMessage({"Operation": "subdata","Name": "PITCH", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/flightmodel/position/theta", "Type": "FLOAT"}})
    time.sleep(0.1)
    client.sendMessage({"Operation": "subdata","Name": "ROLL", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/flightmodel/position/phi", "Type": "FLOAT"}})
    time.sleep(0.1)
    client.sendMessage({"Operation": "subdata","Name": "YAW", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/flightmodel/position/psi", "Type": "FLOAT"}})
    time.sleep(0.1)
    client.sendMessage({"Operation": "subdata","Name": "TIME", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/time/zulu_time_sec", "Type": "FLOAT"}})
    time.sleep(0.1)
    client.sendMessage({"Operation": "subdata","Name": "DATE", "CallbackId": CALLBACK_ID,"Dataref": {"Link": "sim/time/local_date_days", "Type": "INT"}})

def unsubscribe_dataref(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "unsubdata","Name": "LONGITUDE", "CallbackId": CALLBACK_ID})

def unregister_flightloop(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "unregflightloop", "CallbackId": CALLBACK_ID})

def get_function_map() -> dict:
    return {
        "Load Operation DLL"  : load_operations_dll,          #0
        "Unload Operation DLL": unload_operations_dll,        #1
        "Speak": speak,                                       #2
        "Set Smoke ON": set_smoke_on,                         #3
        "Set Smoke OFF": set_smoke_off,                       #4
        "Get Smoke": get_smoke_value,                         #5
        "Register Dataref": register_dataref,                 #6
        "Set Register Dataref On": set_register_dataref_on,   #7
        "Set Register Dataref Off": set_register_dataref_off, #8
        "Load FlightLoop DLL": load_flightloop_dll,           #9
        "Unload FlightLoop DLL": unload_flightloop_dll,       #10
        "Speak 2": speak2,                                    #11
        "Register FlightLoop": register_flightloop,           #12
        "Subscribe Dataref": subscribe_dataref,               #13
        "Unsubscribe Dataref": unsubscribe_dataref,           #14
        "Unregister Flightloop": unregister_flightloop,       #15
    }
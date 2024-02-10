import math
from udp_client_service import UDPClientService
import time
from varname import nameof
from geographiclib.geodesic import Geodesic

CALLBACK_ID = 2847554231936
def get_loaded_function(client: UDPClientService) ->None:
    client.sendMessage({"Operation" : "GetFunctions"})

def load_operations_dll(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "loaddll","Path" : "X-Server.Operations.dll"})

def unload_operations_dll(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "unloaddll","Path" : "X-Server.Operations.dll"})

def speak(client: UDPClientService) -> None:
    client.sendMessage({"Operation" : "Speak", "Text" : "Hello Python"})

def set_smoke_on(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "SetData","Dataref" : {"Link" : "sim/flightmodel/engine/ENGN_thro","Value" : [1.0, 0.5]}})

def set_smoke_off(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "SetData","Dataref" : {"Link" : "sim/operation/failures/rel_smoke_cpit","Value" : 0}})

def get_smoke_value(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "GetData","Dataref" : { "Link" : "sim/operation/failures/rel_smoke_cpit"}})

def register_dataref(client: UDPClientService) -> None:
     client.sendMessage({"Operation": "regdata","Name" : "SMOKE", "Dataref" : {"Link" : "sim/operation/failures/rel_smoke_cpit"}})

def set_register_dataref_on(client: UDPClientService) -> None:
     client.sendMessage({ "Operation": "setregdata","Name" : "SMOKE","Value" : "6"})

def set_register_dataref_off(client: UDPClientService) -> None:
     client.sendMessage({ "Operation": "setregdata","Name" : "SMOKE","Value" : "0"})

def load_beta_dll(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "loaddll","Path" : "X-Server.Operations.Beta.dll"})

def unload_beta_dll(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "unloaddll","Path" : "X-Server.Operations.Beta.dll"})

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

def aquire_planes(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "aquireplanes"})

def release_planes(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "ReleasePlanes"})

def set_plane_count(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "setplanecount", "Count": 2})

def register_plane(client: UDPClientService) -> None:
    client.sendMessage({"Operation": "registerplane", "AircraftModel" : "E:\\X-Plane 11\\Aircraft\\Laminar Research\\Boeing B737-800\\b738.acf"})

def update_tcas(client: UDPClientService) -> None:
    # velocity: float = 0
    # velocity_max : float = 72.0
    # elevation_max: float = 457.2
    # acceleration: float = 2.0
    # delta_time:float = 0.05
    # # track:float = 255.0 
    # while planes["Positions"][0]["Elevation"] < elevation_max:
    #     if(velocity < velocity_max):
    #         velocity = velocity + (acceleration * delta_time)
    #     distance = velocity * delta_time
    #     future = Geodesic.WGS84.Direct(planes["Positions"][0]["Latitude"], planes["Positions"][0]["Longitude"], planes["Positions"][0]["Heading"], distance)
    #     planes["Positions"][0]["Latitude"] = future["lat2"]
    #     planes["Positions"][0]["Longitude"] = future["lon2"]
    #     if (velocity >= velocity_max):
    #         planes["Positions"][0]["Elevation"] += (7.7 * delta_time)
    #         planes["OnGround"][0] = 0
    # for i in range(int(10.0 *  (1.0 / 0.05))):
    client.sendMessage({"Operation": "updateplane",
                        "AircraftId" : 1,
                        "Latitude" : 51.203249,
                        "Longitude": 2.89623,
                        "Elevation" : 5.256982,
                        "Pitch" : 0.0,
                        "Heading" : 255.0,
                        "Roll" : 0.0 })
        # time.sleep(0.05)

def get_function_map() -> dict:
    return {
        nameof(load_operations_dll)  : load_operations_dll,           #0
        nameof(unload_operations_dll): unload_operations_dll,         #1
        # nameof(speak): speak,                                       #2
        nameof(set_smoke_on): set_smoke_on,                           #3
        nameof(set_smoke_off): set_smoke_off,                         #4
        # nameof(get_smoke_value): get_smoke_value,                   #5
        # nameof(register_dataref): register_dataref,                 #6
        # nameof(set_register_dataref_on): set_register_dataref_on,   #7
        # nameof(set_register_dataref_off): set_register_dataref_off, #8
        # nameof(register_flightloop): register_flightloop,           #9
        # nameof(subscribe_dataref): subscribe_dataref,               #10
        # nameof(unsubscribe_dataref): unsubscribe_dataref,           #11
        # nameof(unregister_flightloop): unregister_flightloop,       #12
        # nameof(get_loaded_function): get_loaded_function,           #13
        # nameof(load_beta_dll): load_beta_dll,                       #14
        # nameof(unload_beta_dll): unload_beta_dll,                   #15 
        nameof(aquire_planes): aquire_planes,                         #16
        nameof(set_plane_count): set_plane_count,                     #17   
        nameof(register_plane): register_plane,                       #18   
        nameof(release_planes): release_planes,                       #19 
        nameof(update_tcas): update_tcas,                             #20   
    }
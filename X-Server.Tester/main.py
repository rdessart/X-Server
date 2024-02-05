import logging

# logging.basicConfig(level=logging.DEBUG, format='(%(threadName)-9s) %(message)s',)
logging.basicConfig(filename='main.log', encoding='utf-8',format='%(asctime)s (%(threadName)-9s) %(message)s', level=logging.DEBUG)

def run_as_server() -> int:
    from beacon_service import BeaconService
    from udp_receiver_service import UDPReceiverService    
    logging.info("Server Started...")

    udpListener = UDPReceiverService()
    if(not udpListener.initSerice()):
        logging.critical("Unable to start UDP Reciver Service... EXITING")
        return -1
    udpListener.start()
    
    beaconservice = BeaconService()
    beaconservice.start()
    try:
        input("********PRESS ENTER TO STOP********")
        beaconservice.exit()
        udpListener.exit()
    except KeyboardInterrupt:
        beaconservice.exit()
        udpListener.exit()
    return 0

def run_as_client() -> int:
    from udp_client_service import UDPClientService

    logging.info("Client Started...")
    
    client = UDPClientService()
    if(not client.initSerice()):
        logging.critical("Unable to start UDP Client Service... EXITING")
        return -1
    client.start()
    run : bool = True
    while run:
        try:
            choice = input("""
1: Send Speak Text
2: Activate Smoke
3: Stop Smoke
4: Get Smoke Dataref Status
5: Register Callback
6: Register Dataref
7: Subscribe Dataref
8: Set register dataref ON
9: Set register dataref OFF
10: Load DLL
11: UnLoad DLL
12: Load DLL 2
13: UnLoad DLL 2
14: Speak BETA                       
q:Quit
>>>""")
            print(choice)
        except KeyboardInterrupt:
            run = False
            client.exit()
            break
        match choice:
            case '1':
                logging.debug("Speak")
                client.sendMessage({"Operation": "Speak","Text" : "HELLO Python"})
            case '2':
                logging.debug("Smoke : ON")
                logging.debug(client.sendMessage({"Operation": "SetData","Dataref" : {"Link" : "sim/operation/failures/rel_smoke_cpit","Value" : "6"}}))
            case '3':
                logging.debug("Smoke off")
                client.sendMessage({"Operation": "SetData","Dataref" : {"Link" : "sim/operation/failures/rel_smoke_cpit","Value" : "0"}})
            case '4':
                logging.debug("Get Smoke")
                client.sendMessage({"Operation": "GetData","Dataref" : { "Link" : "sim/operation/failures/rel_smoke_cpit"}})
            case '5':
                logging.debug("Register Callback")
                client.sendMessage({"Operation": "regflightloop", "CallbackInfo" : { "DeltaTime" : 1000, "IsTime": True}})
            case '6':
                logging.debug("Register Dataref")
                client.sendMessage({
                    "Operation": "regdata",
                    "Name" : "SMOKE", 
                    "Dataref" : {
                        "Link" : "sim/operation/failures/rel_smoke_cpit"
                    }})
            case '7':
                logging.debug("Subscribe Dataref")
                client.sendMessage({
                    "Operation": "subdataref",
                     "Name" : "SMOKE", 
                     "CallbackId" : 1
                     })
            case '8':
                logging.debug("set registered Dataref on")
                client.sendMessage({
                    "Operation": "setregdata",
                     "Name" : "SMOKE", 
                     "Value" : "6"
                     })
            case '9':
                logging.debug("set registered Dataref off")
                client.sendMessage({
                    "Operation": "setregdata",
                     "Name" : "SMOKE", 
                     "Value" : "0"
                     })
            case '10':
                logging.debug("load DLL")
                client.sendMessage({
                    "Operation": "loaddll",
                    "Path" : "XServer.Operations.dll"})
            case '11':
                logging.debug("unload DLL")
                client.sendMessage({
                    "Operation": "unloaddll",
                    "Path" : "XServer.Operations.dll"})
            
            case '12':
                logging.debug("load DLL")
                client.sendMessage({
                    "Operation": "loaddll",
                    "Path" : "X-Server.DevOperations.dll"})
            case '13':
                logging.debug("unload DLL")
                client.sendMessage({
                    "Operation": "unloaddll",
                    "Path" : "X-Server.DevOperations.dll"})
            case '14':
                logging.debug("Speak")
                client.sendMessage({"Operation": "Speak2"})
            case 'q':
                run = False
                client.exit()
            case _:
                continue

def main() -> int:
    run_as_client()

if __name__ == "__main__":
    main()
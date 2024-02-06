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
    import client_function

    logging.info("Client Started...")
    
    client = UDPClientService()
    if(not client.initSerice()):
        logging.critical("Unable to start UDP Client Service... EXITING")
        print("Unable to start UDP Client Service... EXITING")
        return -1
    client.start()
    run : bool = True
    input_text = ""
    functions = []
    for id, (key, value) in enumerate(client_function.get_function_map().items()):
        input_text += f"{id:00}. {key}\n"
        functions.append(value)

    input_text += " q. Exit\n>>> "
    while run:
        try:
            choice = input(input_text)
        except KeyboardInterrupt:
            run = False
            client.exit()
            break
        if(choice in ['q', 'Q']):
            run = False
            client.exit()
            break
        functions[int(choice)](client)

def main() -> int:
    run_as_client()

if __name__ == "__main__":
    main()
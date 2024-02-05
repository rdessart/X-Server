import socket
import json
import threading
import logging
import select

class UDPClientService(threading.Thread):
    def __init__(self):
        super(UDPClientService, self).__init__(name="UDPReceiverThread")
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.bindTarget = ("0.0.0.0", 50_558)
        self.simTarget = ("127.0.0.1", 50555)
        self.shoudRun = False

    def initSerice(self) -> bool:
        try:
            self.sock.bind(self.bindTarget)
        except:
            logging.critical(f"Unable to bind on {self.bindTarget[0]}:{self.bindTarget[1]}")
            return False
        self.sock.setblocking(0)
        
        return True

    def exit(self):
        self.shoudRun = False
        logging.debug("exit called")

    def run(self):
        logging.debug("Trying to bind socket")
        
        
        self.shoudRun = True
        logging.debug("Started receiver service")
        while(self.shoudRun):
            ready = select.select([self.sock], [], [], 1)
            if ready[0]:
                data, emitter = self.sock.recvfrom(1024)
                logging.info(f"{emitter} >>> {data.decode()}")
            

        logging.debug("shutting service down...")
        self.sock.close()

    def sendMessage(self, message: dict):
        if not self.shoudRun : 
            return -1
        return self.sock.sendto(json.dumps(message).encode(), self.simTarget)


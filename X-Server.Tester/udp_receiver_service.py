import socket
import json
import threading
import logging
import select

class UDPReceiverService(threading.Thread):
    def __init__(self):
        super(UDPReceiverService, self).__init__(name="UDPReceiverThread")
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.bindTarget = ("0.0.0.0", 50_555)
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
                logging.info(f"{emitter} sent {data.decode()}")
                resp = json.loads(data)
                resp["Result"] = "Ok"
                logging.info(f"{emitter} responding {json.dumps(resp)}")
                self.sock.sendto(json.dumps(resp).encode(), emitter)

        logging.debug("shutting service down...")
        self.sock.close()
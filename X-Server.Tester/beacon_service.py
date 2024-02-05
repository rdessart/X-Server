import socket
import json
import time
import threading
import logging

class BeaconService(threading.Thread):
    def __init__(self):
        super(BeaconService, self).__init__(name="BeaconThread")
        self.beaconOps = {
            "Operation": "Beacon",
            "Author": "Romain DESSART",
            "Description": "Dummy Server",
            "ListeningPort": 50555,
            "IPAddress": "127.0.0.1",
            "Simulator": "Python",
            "SimulatorVersion": 3,
            "SimulatorSDKVersion": 312,
            "Time": None,
        }
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.target = ("127.0.0.1", 50_888)
        self.shoudRun = False


    def exit(self):
        self.shoudRun = False
        logging.debug("exit called")

    def run(self):
        self.shoudRun = True
        logging.debug("Started beacon")
        while(self.shoudRun):
            self.beaconOps["Time"] = int(time.time())
            self.sock.sendto(json.dumps(self.beaconOps).encode(), self.target)
            time.sleep(1)
        logging.debug("shutting service down...")
        self.sock.close()
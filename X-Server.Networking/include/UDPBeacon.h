#pragma once

#include <mutex>

#include "IServer.h"
#include "NetworkUtils.h"
#include <Logger.h>

class UDPBeacon : public IServer
{
public:
    UDPBeacon();
    UDPBeacon(IPInfo ip, int broadcastPort = 50888, int listeningPort=50555);
    int Initalize();
    int SendData(json message);
    json ReceiveMessage();
    void SetIPAddress(IPInfo ip);
    IPInfo GetIPAddress();
protected:
    Logger m_logger = Logger("X-Server.log", "UDPBeacon", true);
    std::vector<IPInfo> _ips;
    std::mutex gLock;
    IPInfo m_ip;
    int m_listeningPort;
    int m_broacastPort;
};
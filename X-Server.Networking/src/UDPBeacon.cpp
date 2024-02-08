#include "../include/UDPBeacon.h"

UDPBeacon::UDPBeacon()
{
    IPInfo ip;
    ip.str_broadcast = "127.0.0.255";
    ip.str_ip = "127.0.0.1";
    ip.str_subnet = "255.255.255.0";
    m_ip = ip;
    m_listeningPort = 50555;
    m_broacastPort = 50888;
}

UDPBeacon::UDPBeacon(IPInfo ip, int broadcastPort, int listeningPort)
{
    _ips = FindIp();
    m_ip = ip;
    m_listeningPort = listeningPort;
    m_broacastPort = broadcastPort;
}

int UDPBeacon::Initalize()
{
    
#ifdef IBM
    if (WSAStartup(MAKEWORD(2, 2), &_wsa))
    {
        m_logger.Log("[X-Server] Unable to initlaize WSA\n");
        return 0x01;
    }
#endif
    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (!ISVALIDSOCKET(_socket))
    {
        m_logger.Log("[X-Server] Socket is invalid\n");
        return 0x02;
    }
#ifdef IBM
    BOOL bOptVal = TRUE;
    int bOptLen = sizeof(BOOL);
    int res = setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, bOptLen);
    if (res == SOCKET_ERROR)
    {
        m_logger.Log("[X-Server]Unable to set socket as broadcast\n");
        return 0x03;
    }

#else
    int broadcast = 1;
    setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);
#endif
    return 0x00;
}

int UDPBeacon::SendData(json message)
{
    message.emplace("Time", std::time(nullptr));
    message.emplace("IPAddress", this->GetIPAddress().str_ip);
    message.emplace("ListeningPort", m_listeningPort);
    //struct addrinfo hints;
    struct sockaddr_in ipTarget;
    int res = inet_pton(AF_INET, GetIPAddress().str_broadcast.c_str(), &ipTarget.sin_addr.s_addr);
    ipTarget.sin_port = htons(m_broacastPort);
    ipTarget.sin_family = AF_INET;

    return sendto(_socket,
        message.dump().c_str(), 
        static_cast<int>(message.dump().length()),
        0, (sockaddr*)&ipTarget,
        static_cast<int>(sizeof(ipTarget)));
}

json UDPBeacon::ReceiveMessage()
{
    return json();
}

void UDPBeacon::SetIPAddress(IPInfo ip)
{
    gLock.lock();
        m_ip = ip;
    gLock.unlock();
}

IPInfo UDPBeacon::GetIPAddress()
{
    gLock.lock();
        IPInfo ip = m_ip;
    gLock.unlock();
    return ip;
}

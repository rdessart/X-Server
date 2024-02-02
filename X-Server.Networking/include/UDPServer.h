#pragma once
#include <queue>

#include "IServer.h"
#include "Message.h"
#include <Logger.h>

class UDPServer
{
public:
	UDPServer();
    ~UDPServer();
    int Initalize();
    int SendData(Message message);
    void ReceiveMessage();
    bool MessagePending();
    bool GetWaitingMessage(Message& outMessage);
protected:
#ifdef IBM
    WSADATA _wsa = { 0 };
#endif
	SOCKET _socket = INVALID_SOCKET;
    Logger m_logger = Logger("X-Server.log", "UDPBeacon", true);
    struct addrinfo* _targetAddress = nullptr;
    fd_set master;
    SOCKET m_maxSocket = INVALID_SOCKET;
    std::queue<Message> m_messageQueue;
};


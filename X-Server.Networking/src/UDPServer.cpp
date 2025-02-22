#include "../include/UDPServer.h"

UDPServer::UDPServer() : m_master({0})
{
}

UDPServer::~UDPServer()
{
    freeaddrinfo(_targetAddress);
}

int UDPServer::Initalize()
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
    struct addrinfo hints;
    memset(&hints, 0x00, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo* bindAddress;
    getaddrinfo("0.0.0.0", "50001", &hints, &bindAddress);
    getaddrinfo("127.0.0.1", "50002", &hints, &_targetAddress);
    if (bind(_socket, bindAddress->ai_addr, static_cast<int>(bindAddress->ai_addrlen)))
    {
        m_logger.Log("Unable to bind on 0.0.0.0:55001");
        int error = GETSOCKETERRNO();
        m_logger.Log("Error: " + std::to_string(error));
        return 0x03;
    }
    freeaddrinfo(bindAddress);
    FD_ZERO(&m_master);
    FD_SET(_socket, &m_master);
    m_maxSocket = _socket;
    return 0x00;
}

int UDPServer::SendData(Message message)
{
    //For performance analysis
    message.message.emplace("ResponseTimestamp", std::time(nullptr));
    std::string text = message.message.dump();
    m_logger.Log("Sending ' :" + text + "'!", Logger::Severity::DEBUG);
    return sendto(_socket, 
        text.c_str(),
        static_cast<int>(text.length()),
        0,
        (sockaddr*)&message.target,
        static_cast<int>(message.target_lenght));
}

void UDPServer::ReceiveMessage()
{
    fd_set read;
    read = m_master;
    while (1)
    {
        int res = select(static_cast<int>(m_maxSocket) + 1, &read, 0, 0, nullptr);
        if (res < 0)
        {
            m_logger.Log("select() has failed " + std::to_string(GETSOCKETERRNO()));
            continue;
        }
        else if (res == 0)
        {
            continue;
        }
        if (FD_ISSET(_socket, &read))
        {
            struct sockaddr_storage cliAddr;
            socklen_t clientLen = sizeof(cliAddr);

            char data[10240];
            int received = recvfrom(_socket, data, 10240, 0, (struct sockaddr*)&cliAddr, &clientLen);
            if (received < 1)
            {
                m_logger.Log("recvfrom() has failed " + std::to_string(GETSOCKETERRNO()));
                continue;
            }
            std::string strData(data);
            strData = strData.substr(0, received);
            json message = json::parse(strData);
            message.emplace("ReceivedTimestamp", std::time(nullptr)); //For performance analysis
            Message m;
            m.message = message;
            m.target = cliAddr;
            m.target_lenght = clientLen;
            m_messageQueue.push(m);
        }
    }
    return;
}

bool UDPServer::MessagePending()
{
    return m_messageQueue.size() > 0;
}

bool UDPServer::GetWaitingMessage(Message& outMessage)
{
    if (!MessagePending())
        return false;
    outMessage = m_messageQueue.front();
    m_messageQueue.pop();
    return true;
}
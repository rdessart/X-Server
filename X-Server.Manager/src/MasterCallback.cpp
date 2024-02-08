#include "../include/MasterCallback.h"

#include "../include/Manager.h"
#include <UDPServer.h>
#include "../include/Managers/OperationManager.h"

void Callback(double step, void* tag)
{
    Manager* manager = (Manager*)tag;
    Message reveivedMessage;
    while (manager->GetServer()->GetWaitingMessage(reveivedMessage))
    {
        manager->GetLogger()->Log("Message : '" + reveivedMessage.message.dump() + "'");
        if (!reveivedMessage.message.contains("Operation")) continue;
        std::string operationName = reveivedMessage.message.value("Operation", "");
        OperationManager* opsManager = (OperationManager*)manager->GetService("OperationManager");
        OperationPointer ops = opsManager->GetOperation(operationName);
        if (ops == nullptr)
        {
            manager->GetLogger()->Log("Operation NOT found : '" + operationName + "'!", Logger::Severity::WARNING);
            return;
        }
        ops(reveivedMessage, manager);
        manager->GetServer()->SendData(reveivedMessage);
    }
}
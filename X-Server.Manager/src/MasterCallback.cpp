#include "../include/MasterCallback.h"

void Callback(double step, void* tag)
{
    OperationParameters* parameters = (OperationParameters*)tag;
    Message reveivedMessage;
    while (parameters->Server->GetWaitingMessage(reveivedMessage))
    {
        parameters->Logger->Log("Message : '" + reveivedMessage.message.dump() + "'");
        if (!reveivedMessage.message.contains("Operation")) continue;
        std::string operationName = reveivedMessage.message.value("Operation", "");
        OperationPointer ops = CallbackOperations::GetOperation(operationName);
        if (ops == nullptr)
        {
            parameters->Logger->Log("Operation NOT found : '" + operationName + "'!", Logger::Severity::WARNING);
            return;
        }
        ops(reveivedMessage, parameters);
        parameters->Server->SendData(reveivedMessage);
    }
}
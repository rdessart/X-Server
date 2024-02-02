#pragma once

#include <Logger.h>
#include <Message.h>

void SpeakOperation(Message& message, MasterCallbackParameter* parameters);
void SetDatarefOperation(Message& message, MasterCallbackParameter* parameters);
void GetDatarefOperation(Message& message, MasterCallbackParameter* parameters);
void RegisterDatarefOperation(Message& message, MasterCallbackParameter* parameters);
void SetRegisteredDatarefOperation(Message& message, MasterCallbackParameter* parameters);
void GetRegisteredDatarefOperation(Message& message, MasterCallbackParameter* parameters);
void GetDatarefInfoOperation(Message& message, MasterCallbackParameter* parameters);
void GetRegisteredDatarefInfoOperation(Message& message, MasterCallbackParameter* parameters);
void RegisterFlightLoopOperation(Message& m, MasterCallbackParameter* parameters);
void SubscribeDatarefOperation(Message& m, MasterCallbackParameter* parameters);
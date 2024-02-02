#pragma once
#include <map>
#include <string>

#include <Message.h>
#include "../OperationParameters.h"

typedef void(*OperationPointer)(Message&, struct OperationParameters*);
typedef int(*OperationLoader)(std::map<std::string, std::string>*);

class OperationManager
{
public:
	OperationManager();
	~OperationManager();
	int DoLoadDLL(std::string path);
	int DoUnloadDll(std::string path);

	OperationPointer GetOperation(std::string key);
	

protected:
	std::map<std::string, OperationPointer> m_loadedFunctions;
	std::map<std::string, HINSTANCE> m_linkedDLL;
};

static void LoadDLL(Message& message, OperationParameters* parameters);
static void UnLoadDLL(Message& message, OperationParameters* parameters);


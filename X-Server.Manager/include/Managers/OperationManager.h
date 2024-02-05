#pragma once
#include <map>
#include <string>
#include <Logger.h>
#include <Message.h>
#ifndef IBM
#include <dlfcn.h>
#endif
#include "../Manager.h"

typedef int(*DLLInitialize)(class Manager*);
typedef void(*OperationPointer)(Message&, class Manager*);
typedef int(*OperationLoader)(std::map<std::string, std::string>*);

class OperationManager
{
public:
	OperationManager();
	~OperationManager();
	int DoLoadDLL(std::string path, Manager* manager);
	int DoUnloadDll(std::string path, Manager* manager);

	OperationPointer GetOperation(std::string key);
	

protected:
	std::map<std::string, OperationPointer> m_loadedFunctions;
	#ifdef IBM
	std::map<std::string, HINSTANCE> m_linkedDLL;
	#else
	std::map<std::string, void*> m_linkedDLL;
	#endif
	Logger m_logger;
};

static void LoadDLL(Message& message, Manager* parameters);
static void UnLoadDLL(Message& message, Manager* parameters);


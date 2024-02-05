#include "../../include/Managers/OperationManager.h"

OperationManager::OperationManager() :
	m_linkedDLL(),
	m_logger("X-Server", "OperationManager", false)
{
	m_loadedFunctions["loaddll"] = LoadDLL;
	m_loadedFunctions["unloaddll"] = UnLoadDLL;
}

OperationManager::~OperationManager()
{
}

int OperationManager::DoLoadDLL(std::string path, Manager* manager)
{
	if (m_linkedDLL.contains(path)) { //DLL is already in memory
		return -1;
	}
	
	#ifdef IBM
		HINSTANCE hDLL = LoadLibraryA(path.c_str());
	#else
		void* hDLL = dlopen(path.c_str(), RTLD_LAZY);
	#endif

	if (!hDLL) {
		//Unable to find the file
		m_logger.Log("Unable to load DLL with path : '" + path + "'!", Logger::Severity::WARNING);
		return -2;
	}

	m_linkedDLL.emplace(path, hDLL);
	//We try to get the init function.
	#ifdef IBM
		DLLInitialize initalizeDll = (DLLInitialize)GetProcAddress(m_linkedDLL[path], "InitializeDLL");
	#else
		DLLInitialize initalizeDll = (DLLInitialize)dlsym(m_linkedDLL[path], "InitializeDLL");
	#endif
	if (!initalizeDll)
	{
		m_logger.Log("DLL : '" + path + "' didn't contained a InitializeDLL function!", Logger::Severity::INFO);
	}
	else {
		initalizeDll(manager);
	}
	//We try to get the loader function.
	#ifdef IBM
		OperationLoader loader = (OperationLoader)GetProcAddress(m_linkedDLL[path], "GetOperations");
	#else
		OperationLoader loader = (OperationLoader)dlsym(m_linkedDLL[path], "GetOperations");
	#endif
	
	if (!loader) {
		m_logger.Log("DLL : '" + path + "' didn't contained a GetOperation function! Abording", Logger::Severity::WARNING);
		//Didn't find the function or it don't have the right definition.
		return -3;
	}
	std::map<std::string, std::string> functionsNames;
	int loadedCount = loader(&functionsNames);
	if (loadedCount <= 0) {
		// we didn't load any function or they was an import error.
		// loading no function can mean the name are the same as the previous.
		m_logger.Log("DLL : '" + path + "' No function where loaded, function probably has same operation name as operations allready added.", Logger::Severity::WARNING);
		return -4;
	}

	std::size_t sizeBefore = m_loadedFunctions.size();
	for (auto& kv : functionsNames)
	{
		if (!m_loadedFunctions.contains(kv.first)) { //We skip function loading if it has the same name.
			// m_loadedFunctions[kv.first] = (OperationPointer)GetProcAddress(m_linkedDLL[path], kv.second.c_str());
			#ifdef IBM
				m_loadedFunctions[kv.first] = (OperationPointer)GetProcAddress(m_linkedDLL[path],  kv.second.c_str());
			#else
				m_loadedFunctions[kv.first] = (OperationPointer)dlsym(m_linkedDLL[path],  kv.second.c_str());
			#endif
		}
	}

	return static_cast<int>(m_loadedFunctions.size() - sizeBefore);
}

int OperationManager::DoUnloadDll(std::string path, Manager* manager)
{
	if (!m_linkedDLL.contains(path)) return -1;

#ifdef IBM
	DLLInitialize uninitalizeDll = (DLLInitialize)GetProcAddress(m_linkedDLL[path], "UninitializeDLL");
#else
	DLLInitialize uninitalizeDll = (DLLInitialize)dlsym(m_linkedDLL[path], "UninitializeDLL");
#endif
	if (!uninitalizeDll)
	{
		m_logger.Log("DLL : '" + path + "' didn't contained a InitializeDLL function!", Logger::Severity::INFO);
	}
	else {
		uninitalizeDll(manager);
	}

	//We get back all function linked with the DLL
	std::map<std::string, std::string> functionsNames;
	#ifdef IBM
		OperationLoader loader = (OperationLoader)GetProcAddress(m_linkedDLL[path], "GetOperations");
	#else
		OperationLoader loader = (OperationLoader)dlsym(m_linkedDLL[path], "GetOperations");
	#endif
	if (!loader) {
		//Didn't find the function or it don't have the right definition.
		return -3;
	}
	int loadedCount = loader(&functionsNames);
	if (loadedCount <= 0) {
		// we didn't load any function or they was an import error.
		// loading no function can mean the name are the same as the previous.
		return -4;
	}

	for (auto& kv : functionsNames)
	{
		if (!m_loadedFunctions.contains(kv.first)) continue;
		m_loadedFunctions.erase(kv.first);
	}
	#ifdef IBM
		FreeLibrary(m_linkedDLL[path]);
	#else
		dlclose(m_linkedDLL[path]);
	#endif
	m_linkedDLL.erase(path);
	return 0;
}

OperationPointer OperationManager::GetOperation(std::string key)
{
	std::transform(key.begin(), key.end(), key.begin(),[](unsigned char c) { return std::tolower(c); });
	if (!m_loadedFunctions.contains(key)) return nullptr;
	return m_loadedFunctions[key];
}

static void LoadDLL(Message& message, Manager* manager)
{
	if (!message.message.contains("Path"))
	{
		message.message["Result"] = "Error:Missing Path information with dll path";
		return;
	}

	std::string path = message.message["Path"].get<std::string>();
	OperationManager* opsManager = (OperationManager*)manager->GetService("OperationManager");
	int res = opsManager->DoLoadDLL(path, manager);
	if (res < 0)
	{
		message.message["Result"] = "Error:DLL Load returned an error code : " + std::to_string(res);
		return;
	}
	if (res == 0)
	{
		message.message["Result"] = "Error:DLL Load was Ok but not function where loaded. Check for duplicate name in your DLLs";
		return;
	}
	message.message["Result"] = "Ok";
}

static void UnLoadDLL(Message& message, Manager* manager)
{
	if (!message.message.contains("Path"))
	{
		message.message["Result"] = "Error:Missing Path information with dll path";
		return;
	}
	std::string path = message.message["Path"].get<std::string>();
	OperationManager* opsManager = (OperationManager*)manager->GetService("OperationManager");
	int res = opsManager->DoUnloadDll(path, manager);
	if (res < 0)
	{
		message.message["Result"] = "Error:DLL Load returned an error the DLL was not loaded";
		return;
	}
	message.message["Result"] = "Ok";
}

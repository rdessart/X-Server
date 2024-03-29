#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

class Logger
{
	/*
	* Class used to manage logger object.
	* Logger output a "csv" style log (data separated by a tabulation")
	*/
public:
	enum class Severity {
		TRACE, // Basic information
		DEBUG, // Debug information
		INFO,
		WARNING, // Cautionous information
		CRITICAL, // Information are critical but program can keep running
		FATAL // Crashed
	};
	Logger();
	Logger(std::string filename, std::string module = "", bool erease = false);
	~Logger();
	std::string GetModuleName();
	void SetModuleName(std::string module);
	virtual void Log(std::string message, Logger::Severity severity = Logger::Severity::DEBUG) const;
	virtual void Log(std::string message, std::string module, Logger::Severity severity = Logger::Severity::DEBUG) const;
	const char* CurrentDateTime() const;
	void operator+=(const std::string& message);
protected:
	std::string getSeverityStr(Logger::Severity severity) const;
	std::string m_module;
	std::ofstream* m_logfile;
};
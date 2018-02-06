#include "CoutConsoleProvider.h"
#include <iostream>
#include <string>

using namespace Core;

CoutConsoleProvider::CoutConsoleProvider() : 
	ConsoleProvider()
{
	_state = CONSOLE_PROVIDER_INITIALISED;
}

bool CoutConsoleProvider::log(const std::string & message)
{
	std::cout << message.c_str();
	return true;
}

bool CoutConsoleProvider::logLine(const std::string & message)
{
	std::cout << message.c_str() << std::endl;
	return true;
}

bool Core::CoutConsoleProvider::log(const std::string & message, LogLevels level)
{
	return log(_levelNames[level] + message);
}

bool Core::CoutConsoleProvider::logLine(const std::string & message, LogLevels level)
{
	return logLine(_levelNames[level] + message);
}

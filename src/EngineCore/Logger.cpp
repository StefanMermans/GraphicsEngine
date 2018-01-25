#include "Logger.h"

#ifdef _WIN32
#include "WinConsoleProvider.h"
#else
#include "CoutConsoleProvider.h"
#endif


using namespace Core;

ConsoleProvider * Logger::_consoleProviderPtr = NULL;

bool Logger::init()
{
#ifdef _WIN32
	_consoleProviderPtr = new WinConsoleProvider();
#else
	_consoleProviderPtr = new CoutConsoleProvider();
#endif

	return _consoleProviderPtr->getState() == CONSOLE_PROVIDER_INITIALISED;
}

bool Logger::log(const std::string & message) {
	if (_consoleProviderPtr == NULL) {
		return false;
	}

	return _consoleProviderPtr->log(message);
}

bool Logger::logLine(const std::string & message)
{
	if (_consoleProviderPtr == NULL) {
		return false;
	}

	return _consoleProviderPtr->logLine(message);
}

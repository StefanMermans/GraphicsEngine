#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#include "ConsoleProvider.h"



namespace Core {
	class Logger {
	public:
		static bool init(bool showLevel = true, bool forceCout = false);
		
		static bool error(const std::string &message);
		static bool errorLine(const std::string &message);

		static bool warn(const std::string &message);
		static bool warnLine(const std::string &message);

		static bool log(const std::string &message);
		static bool logLine(const std::string &message);
	private:
		static ConsoleProvider * _consoleProviderPtr;
		static bool _showLevel;
	};

}

#endif // !LOGGER_H

#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>


namespace Core {

	enum LogLevels
	{
		ERR,
		WARN,
		INFO,
		DEBUG
	};

	static const unsigned int CONSOLE_PROVIDER_FAILED;
	static const unsigned int CONSOLE_PROVIDER_INITIALISED;
	static const unsigned int CONSOLE_PROVIDER_IDLE;

	class ConsoleProvider
	{
	public:
		ConsoleProvider();
		virtual ~ConsoleProvider() = default;

		virtual bool log(const std::string &message) = 0;
		virtual bool logLine(const std::string &message) = 0;

		virtual bool log(const std::string &message, LogLevels level) = 0;
		virtual bool logLine(const std::string &message, LogLevels level) = 0;

		unsigned int getState() const;
	protected:
		unsigned int _state;
	};
}

#endif // !CONSOLE_H

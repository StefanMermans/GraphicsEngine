#ifndef WIN_CONSOLE_PROVIDER_H
#define WIN_CONSOLE_PROVIDER_H

#ifdef _WIN32

#include <Windows.h>
#include <string>

#include "ConsoleProvider.h"

namespace Core {

	class WinConsoleProvider : public ConsoleProvider
	{
	public:
		WinConsoleProvider();
		~WinConsoleProvider() = default;
	private:
		bool log(const std::string &message) override;
		bool logLine(const std::string &message) override;
		
		bool log(const std::string &message, LogLevels level) override;
		bool logLine(const std::string &message, LogLevels level) override;
		
		HANDLE _stdHandle;

		const std::string _levelNames[4] = {
			"[error]",
			"[warn] ",
			"[info] ",
			"[debug]"
		};
	};

}

#endif // _WIN32
#endif // !WIN_CONSOLE_PROVIDER_H

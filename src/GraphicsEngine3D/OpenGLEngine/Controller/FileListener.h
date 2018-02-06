#ifndef FILE_LISTENER_H
#define FILE_LISTENER_H

#include <string>
#include <Windows.h>

namespace FILE_LISTENER{
	enum FileListenerState
	{
		UNINTIALISED,
		FAILED,
		READY,
		OLD_HANDLE
	};
}


class FileListener {
public:
	FileListener(const std::string &folder);
	FileListener(const LPCWSTR & folder);
	~FileListener();

	bool isInitialised();
	bool listen(DWORD timeout = INFINITE);
private:
	bool init();

	FILE_LISTENER::FileListenerState _state;

	HANDLE _handle;
	LPCWSTR _folder;
};

#endif // !FILE_LISTENER_H

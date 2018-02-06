#include "FileListener.h"
#include <Windows.h>
#include "../../Core/Logger.h"

using Core::Logger;
using FILE_LISTENER::FileListenerState;

FileListener::FileListener(const std::string &folder) 
{	
	_handle = NULL;
	_state = FileListenerState::UNINTIALISED;
	_folder = LPCWSTR(folder.c_str());

	init();
}

FileListener::FileListener(const LPCWSTR & folder) :
	_folder(folder)
{
	_handle = NULL;
	_state = FileListenerState::UNINTIALISED;

	init();
}

FileListener::~FileListener()
{
	if (isInitialised()) {
		// I assume this function will delete the content of the handle
		// and with it clear up any memory it uses.
		FindCloseChangeNotification(_handle);
	}
}

bool FileListener::isInitialised()
{
	return (_handle != INVALID_HANDLE_VALUE && _handle != NULL);
}

bool FileListener::listen(DWORD timeout)
{
	if (_state == FileListenerState::OLD_HANDLE) {
		init();
	}
	
	if (!isInitialised()) {
		_state = FileListenerState::FAILED;
		return false;
	}
	
	if (WaitForSingleObject(_handle, timeout) == WAIT_TIMEOUT) {
		_state = FileListenerState::FAILED;
		return false;
	}

	_state = FileListenerState::OLD_HANDLE;
	return true;
}

bool FileListener::init() {
	if (_state == FileListenerState::UNINTIALISED || _state == FileListenerState::FAILED) {
		_handle = FindFirstChangeNotification(_folder, true, FILE_NOTIFY_CHANGE_LAST_WRITE);
		_state = FileListenerState::READY;
		return true;
	}
	else {
		_state = FileListenerState::READY;
		return FindNextChangeNotification(_handle);
	}
}

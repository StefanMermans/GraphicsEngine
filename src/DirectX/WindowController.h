#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <d3d12.h>

#include "DirectXController.h"

class WindowController
{
public:
	WindowController(HINSTANCE hInstance, INT nCmdShow);
	~WindowController();

	bool init(DirectXController * controllerPtr);
	bool update();

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	HINSTANCE _hInstance;
	INT _nCmdShow;
	MSG _msg;
	HWND _hwnd;
};

#endif // !WINDOW_H

#include "..\stdafx.h"
#include "WindowController.h"
#include "..\Core\Logger.h"

using Core::Logger;

bool WindowController::init(DirectXController * controllerPtr)
{
	// Define and create the window
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = (WNDPROC) WindowProc;
	windowClass.hInstance = _hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"ClientClass";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 
		0,
		0,
		static_cast<LONG>(controllerPtr->getWidth()),
		static_cast<LONG>(controllerPtr->getHeight())
	};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	_hwnd = CreateWindow(
		windowClass.lpszClassName,
		LPCWSTR(controllerPtr->getTitle().c_str()),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, // No parent window.
		nullptr, // No menus.
		_hInstance,
		controllerPtr
	);

	// Check initialisation.
	if (_hwnd == NULL) {
		Logger::logLine("Failed to initialise window!");
		system("PAUSE");
		return false;
	}
	
	controllerPtr->init(&_hwnd);
	ShowWindow(_hwnd, _nCmdShow);
	
	return true;
}

bool WindowController::update()
{
	if (_msg.message == WM_QUIT) {
		return false;
	}

	if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
	return true;
}

WindowController::WindowController(HINSTANCE hInstance, INT nCmdShow) {
	_hInstance = hInstance;
	_nCmdShow = nCmdShow;
	_hwnd = nullptr;
	_msg = { 0 };
}

WindowController::~WindowController() {

}

LRESULT WindowController::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DirectXController * controllerPtr = reinterpret_cast<DirectXController*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message) {
	case WM_CREATE:
		{
			// Save the WindowEventHandler* passed in to CreateWindow.
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		}
		return 0;
	case WM_KEYDOWN:
		controllerPtr->keyDown(static_cast<UINT8>(wParam));
		return 0;
	case WM_KEYUP:
		controllerPtr->keyUp(static_cast<UINT8>(wParam));
		return 0;
	case WM_PAINT:
		controllerPtr->draw();
		return 0;
	case WM_DESTROY:
		controllerPtr->destroy();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

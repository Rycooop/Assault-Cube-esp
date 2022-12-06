#ifndef AC_WINDOW_H
#define AC_WINDOW_H

#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <dwmapi.h>

#include "utility.h" 
#include "vector.h"

#pragma comment(lib, "dwmapi.lib")


class WindowManager {
public:
	WindowManager() = default;
	~WindowManager() = default;

	bool w_Initialize(const HINSTANCE hInstance = 0);
	void w_HandleWindowMessages();
	void w_Shutdown();

	inline HWND GetOverlayHWND() { return this->overlayHWND; }
	inline Vector2 w_GetWindowCoordinates() { return this->WindowCoordinates; }
	inline Vector2 w_GetWindowDimensions() { return this->WindowDimensions; }

private:
	bool w_CreateWindow();
	static long w_WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	HINSTANCE hInstance;
	HWND overlayHWND;
	bool m_isRunning;

	Vector2 WindowCoordinates, WindowDimensions;
};

#endif //AC_WINDOW_H
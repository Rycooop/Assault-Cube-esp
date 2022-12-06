#include "window.h"


bool WindowManager::w_Initialize(const HINSTANCE hInstance) {
	this->hInstance = hInstance;
	this->m_isRunning = true;

	if (!this->w_CreateWindow()) {
		std::cerr << "[-] Failed to create window" << std::endl;
		return false;
	}

	return true;
}

void WindowManager::w_HandleWindowMessages() {
	MSG msg;

	while (this->m_isRunning) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			this->m_isRunning = false;
			break;
		}
	}
}

void WindowManager::w_Shutdown() {

}

bool WindowManager::w_CreateWindow() {
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.hInstance = this->hInstance;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"CheatClass";
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wc.hCursor = LoadCursor(this->hInstance, IDC_ARROW);
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = (WNDPROC)this->w_WindowProc;
	RegisterClassEx(&wc);


	RECT TargetRect;
	if (GetWindowRect(FindWindowA(NULL, "AssaultCube"), &TargetRect)) {
		this->WindowCoordinates = Vector2(TargetRect.left, TargetRect.top);
		this->WindowDimensions = Vector2(TargetRect.right - TargetRect.left, TargetRect.bottom - TargetRect.top);
	}
	else {
		this->WindowCoordinates = Vector2(10, 10);
		this->WindowDimensions = Vector2(200, 200);
	}

	this->overlayHWND = CreateWindowEx(WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST, L"CheatClass", L"Cheat Overlay", WS_POPUP, this->WindowCoordinates.x, this->WindowCoordinates.y, this->WindowDimensions.x, this->WindowDimensions.y, NULL, NULL, this->hInstance, NULL);

	DWM_BLURBEHIND bb;
	bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	bb.fEnable = true;
	bb.fTransitionOnMaximized = true;
	bb.hRgnBlur = CreateRectRgn(0, 0, -1, -1);
	DwmEnableBlurBehindWindow(this->overlayHWND, &bb);
	SetLayeredWindowAttributes(this->overlayHWND, NULL, NULL, NULL);

	ShowWindow(this->overlayHWND, 1);

	return true;
}

long WindowManager::w_WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY: return 0;
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

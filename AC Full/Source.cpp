#include <iostream>
#include <Windows.h>
#include <thread>
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>
#include "drawing.h"
#include "util.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

LPDIRECT3D9			d3d;
LPDIRECT3DDEVICE9	d3ddev;

HANDLE pHandle;
HWND targetHwnd, overlayHwnd;
Vec2 vecScreen;
int windowWidth, windowHeight, windowX, windowY;
bool bMenuShow = false, bEsp = false, bSnapLines = false, bEspBox = false, bHeadBoxes = false, bShadedBoxes = false;

D3DCOLOR espColor = D3DCOLOR_ARGB(255, 0, 255, 255);

DWORD modBase;
DWORD localPlayer;
DWORD entityList;
DWORD* dwViewMatrix;

void initD3D(HWND hwnd);
void renderFrame(void);
void cleanD3D(void);
void espThread();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

struct ents
{
	DWORD ent[32];
	int health[32];
	Vec3 pos[32];
	Vec3 headPos[32];
	Vec3 angles[32];
	Vec2 ent2D[32];
	Vec2 entHead2D[32];
	float viewMatrix[16];
	char entName[16];
}mainInfo;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow)
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "conout$", "w", stdout);
	std::cout << "[*] Loading..." << std::endl;
	std::cout << "[*] Launch Game..." << std::endl;

	while (!targetHwnd)
	{
		targetHwnd = FindWindowA(NULL, "AssaultCube");
	}

	RECT rect;
	GetWindowRect(targetHwnd, &rect);
	windowWidth = rect.right - rect.left;
	windowHeight = rect.bottom - rect.top;
	windowX = rect.left;
	windowY = rect.top;
	vecScreen.x = windowWidth / 2;
	vecScreen.y = windowHeight;


	DWORD procId;
	GetWindowThreadProcessId(targetHwnd, &procId);
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

	if (pHandle == INVALID_HANDLE_VALUE)
		return 0;

	std::cout << "Width: " << windowWidth << " Height: " << windowHeight << std::endl;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize =				sizeof(WNDCLASSEX);
	wc.style =				CS_HREDRAW | CS_VREDRAW;
	wc.hInstance =			hInstance;
	wc.lpfnWndProc =		WindowProc;
	wc.lpszClassName =		L"ACCLASS";
	wc.hbrBackground =		CreateSolidBrush(RGB(0, 0, 0));
	wc.hCursor =			LoadCursor(hInstance, IDC_CROSS);
	RegisterClassEx(&wc);

	overlayHwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, L"ACCLASS", L"Overlay Cheat", WS_POPUP, windowX, windowY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

	DWM_BLURBEHIND bb;
	bb.dwFlags =					DWM_BB_ENABLE | DWM_BB_BLURREGION;
	bb.fEnable =					true;
	bb.fTransitionOnMaximized =		true;
	bb.hRgnBlur =					CreateRectRgn(0, 0, -1, -1);
	DwmEnableBlurBehindWindow(overlayHwnd, &bb);
	SetLayeredWindowAttributes(overlayHwnd, NULL, NULL, NULL);

	ShowWindow(overlayHwnd, nCmdShow);

	MSG msg;

	initD3D(overlayHwnd);
	if (!draw::deviceInit(d3ddev))
	{
		Sleep(2000);
		draw::deviceInit(d3ddev);
	}

	//FreeConsole();
	//fclose(f);

	StaticGameInfo gameInfo = StaticGameInfo(pHandle, procId, L"ac_client.exe");
	modBase =		gameInfo.moduleBase;
	localPlayer =	gameInfo.localPlayer;
	entityList =	gameInfo.entityList;
	dwViewMatrix =	gameInfo.dwViewMatrix;

	std::cout << modBase << std::endl;
	std::cout << localPlayer << std::endl;
	std::cout << entityList << std::endl;
	std::cout << dwViewMatrix << std::endl;

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)espThread, 0, 0, 0);

	while (true)
	{

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;

		RECT rect;
		GetWindowRect(targetHwnd, &rect);
		windowWidth = rect.right - rect.left;
		windowHeight = rect.bottom - rect.top;
		windowX = rect.left;
		windowY = rect.top;
		MoveWindow(overlayHwnd, windowX, windowY, windowWidth, windowHeight, true);

		renderFrame();
	}

	cleanD3D();
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		return 0;

	}break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void initD3D(HWND hwnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed =			true;
	d3dpp.hDeviceWindow =		hwnd;
	d3dpp.SwapEffect =			D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat =	D3DFMT_A8R8G8B8;
	d3dpp.BackBufferWidth =		windowWidth;
	d3dpp.BackBufferHeight =	windowHeight;

	HRESULT devCreated = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

	if (devCreated != S_OK)
	{
		return;
	}
}

void renderFrame(void)
{
	d3ddev->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene();


	draw::drawHeaderText("Assault Cube Hack by Rycoop!", vecScreen.x, vecScreen.y - 50, D3DCOLOR_ARGB(255, 255, 0, 255));

	for (int i = 1; i < 32; i++)
	{

		if (mainInfo.ent[i] == NULL || mainInfo.ent[i] == 0 || mainInfo.ent[i] == localPlayer || mainInfo.health[i] <= 0 || mainInfo.health[i] > 100)
			continue;

		if (util::worldToScreen(mainInfo.pos[i], mainInfo.ent2D[i], mainInfo.viewMatrix, windowWidth, windowHeight) &&
			util::worldToScreen(mainInfo.headPos[i], mainInfo.entHead2D[i], mainInfo.viewMatrix, windowWidth, windowHeight))
		{
			draw::drawEspBox(mainInfo.ent2D[i], mainInfo.entHead2D[i], mainInfo.health[i], espColor);
			draw::drawLine(vecScreen, mainInfo.ent2D[i], 1, espColor);

			ReadProcessMemory(pHandle, (LPCVOID)(mainInfo.ent[i] + 0x225), &mainInfo.entName, sizeof(mainInfo.entName), NULL);

			if (mainInfo.entName[0] != NULL)
				draw::drawInfoBox(mainInfo.entHead2D[i], mainInfo.entName, "30m", D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	if (bMenuShow)
	{
		SetWindowLongPtr(overlayHwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED);
		draw::menuManager();
	}
	else SetWindowLongPtr(overlayHwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED);

	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void cleanD3D(void) {
	d3ddev->Release();
	d3d->Release();
} 

void espThread()
{

	while (true)
	{

		if (GetAsyncKeyState(VK_INSERT) & 1)
			bMenuShow = !bMenuShow;

		ReadProcessMemory(pHandle, (float*)(dwViewMatrix), &mainInfo.viewMatrix, sizeof(mainInfo.viewMatrix), NULL);

		for (int i = 1; i < 32; i++)
		{
			ReadProcessMemory(pHandle, (DWORD*)(entityList + (0x4 * i)), &mainInfo.ent[i], sizeof(DWORD), NULL);
			ReadProcessMemory(pHandle, (int*)(mainInfo.ent[i] + 0xF8), &mainInfo.health[i], sizeof(int), NULL);

			if (mainInfo.ent[i] == NULL || mainInfo.health[i] <= 0 || mainInfo.health[i] > 100)
				continue;

			ReadProcessMemory(pHandle, (Vec3*)(mainInfo.ent[i] + 0x4), &mainInfo.headPos[i], sizeof(Vec3), NULL);
			ReadProcessMemory(pHandle, (Vec3*)(mainInfo.ent[i] + 0x34), &mainInfo.pos[i], sizeof(Vec3), NULL);
			ReadProcessMemory(pHandle, (Vec3*)(mainInfo.ent[i] + 0x40), &mainInfo.angles[i], sizeof(Vec3), NULL);

		}
	}
}

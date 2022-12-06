#include <iostream>
#include "app.h"
#include "utility.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "conout$", "w", stdout);
	freopen_s(&f, "conout$", "w", stderr);

	App* app = new App();
	
	if (!app->Initialize(hInstance)) {
		Sleep(3000);
		return 0;
	}
	else {
		app->Run();
	}

	app->Shutdown();

	return 0;
}
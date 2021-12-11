#pragma once
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "globals.h"

namespace util
{
	bool worldToScreen(Vec3 pos, Vec2& screen, float viewMatrix[16], int windowWidth, int windowHeight);
	DWORD GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
	Vec3 subtract(Vec3 src, Vec3 dst);
	float Distance(Vec3 src, Vec3 dst);
}

class StaticGameInfo
{
public:
	DWORD moduleBase;
	DWORD localPlayer;
	DWORD entityList;
	DWORD* dwViewMatrix;

	StaticGameInfo() {};

	StaticGameInfo(HANDLE hProc, DWORD procId, const wchar_t* moduleName)
	{
		this->moduleBase = util::GetModuleBaseAddress(procId, moduleName);
		ReadProcessMemory(hProc, (DWORD*)(this->moduleBase + 0x10F4F4), &this->localPlayer, sizeof(DWORD), NULL);
		ReadProcessMemory(hProc, (DWORD*)(this->moduleBase + 0x10F4F8), &this->entityList, sizeof(DWORD), NULL);
		this->dwViewMatrix = (DWORD*)(moduleBase + 0x101AE8);
	}
	void updateInfo()
	{

	}
};


#include "utility.h"


std::mutex Utility::Mut;
std::mutex Utility::ConsoleMutex;
std::condition_variable Utility::cv;
bool Utility::IsCVReady = false;

std::uintptr_t Utility::GetModuleBase(const std::wstring& ModuleName, const DWORD& ProcessID) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
	MODULEENTRY32 ModEntry;
	ModEntry.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(hSnap, &ModEntry)) {
		return 0;
	}

	if (!_wcsicmp(ModuleName.c_str(), ModEntry.szModule)) {
		return (std::uintptr_t)ModEntry.modBaseAddr;
	}

	while (Module32Next(hSnap, &ModEntry)) {
		if (!_wcsicmp(ModuleName.c_str(), ModEntry.szModule)) {
			return (std::uintptr_t)ModEntry.modBaseAddr;
		}
	}

	return 0;
}


HANDLE Utility::GetHandleByProcessName(const std::wstring& ProcessName) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &ProcEntry)) {
		return 0;
	}

	if (wcsstr(ProcEntry.szExeFile, ProcessName.c_str())) {
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcEntry.th32ProcessID);
	}

	while (Process32Next(hSnap, &ProcEntry)) {
		if (wcsstr(ProcEntry.szExeFile, ProcessName.c_str())) {
			return OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcEntry.th32ProcessID);
		}
	}

	return 0;
}

bool Utility::WorldToScreen(const Vector3& Position, std::vector<float> ViewMatrix, const Vector2& ScreenDimensions, Vector2& Screen) {
	Vector4 ClipCoords;
	ClipCoords.x = Position.x * ViewMatrix.at(0) + Position.y * ViewMatrix.at(4) + Position.z * ViewMatrix.at(8) + ViewMatrix.at(12);
	ClipCoords.y = Position.x * ViewMatrix.at(1) + Position.y * ViewMatrix.at(5) + Position.z * ViewMatrix.at(9) + ViewMatrix.at(13);
	ClipCoords.z = Position.x * ViewMatrix.at(2) + Position.y * ViewMatrix.at(6) + Position.z * ViewMatrix.at(10) + ViewMatrix.at(14);
	ClipCoords.w = Position.x * ViewMatrix.at(3) + Position.y * ViewMatrix.at(7) + Position.z * ViewMatrix.at(11) + ViewMatrix.at(15);

	if (ClipCoords.w < 0.1f)
		return false;
	
	Vector3 NDC;
	NDC.x = ClipCoords.x / ClipCoords.w;
	NDC.y = ClipCoords.y / ClipCoords.w;
	NDC.z = ClipCoords.z / ClipCoords.w;

	Screen.x = (ScreenDimensions.x / 2 * NDC.x) + (NDC.x + ScreenDimensions.x / 2);
	Screen.y = -(ScreenDimensions.y / 2 * NDC.y) + (NDC.y + ScreenDimensions.y / 2);
	return true;
}
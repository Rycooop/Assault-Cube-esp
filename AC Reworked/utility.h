#ifndef AC_UTILITY_H
#define AC_UTILITY_H

#include <iostream>
#include <cstdint>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <mutex>
#include <condition_variable>

#include "vector.h"


class Utility {
public:
	static std::mutex Mut, ConsoleMutex;
	static std::condition_variable cv;
	static bool IsCVReady;

	static std::uintptr_t GetModuleBase(const std::wstring& ModuleName, const DWORD& ProcessID);
	static HANDLE GetHandleByProcessName(const std::wstring& ProcessName);

	template <typename Type> static bool WriteToProcess(HANDLE hProcess, const Type& Value, const std::uintptr_t Addr, std::size_t Size) {
		if (!Addr || !Size)
			return false;

		if (WriteProcessMemory(hProcess, Addr, &Value, Size, NULL)) {
			return true;
		}
		else return false;
	}

	inline static void PrintError(const std::string& Error) { 
		std::unique_lock<std::mutex> Lock(ConsoleMutex);
		std::cerr << "[-] " << Error << std::endl;
	}

	inline static HWND GetTargetHWND(const std::string& WindowName) { return FindWindowA(NULL, WindowName.c_str()); }

	static bool WorldToScreen(const Vector3& Position, std::vector<float> ViewMatrix, const Vector2& ScreenDimensions, Vector2& Screen);
};

#endif //AC_UTILITY_H


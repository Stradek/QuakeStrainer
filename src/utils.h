#pragma once

#include <windows.h>
#include <iostream>
#include <cassert>

namespace PatchingUtils
{
    class SmartProcessHandle;

	extern void EnableDebugPriv();
    extern DWORD GetProcessIdByProcessName(const char* processName);

    extern DWORD_PTR GetProcessBaseAddress(HANDLE hProcess);
    extern DWORD_PTR GetModuleBaseAddress(HANDLE hProcess, const char* moduleName);
    extern DWORD_PTR GetRelativeAddress(HANDLE hProcess, DWORD_PTR address);

    extern DWORD_PTR ReadMemoryRelative(HANDLE hProcess, DWORD_PTR address);
    extern DWORD_PTR ReadMemoryGlobal(HANDLE hProcess, DWORD_PTR address);
    
    template<typename T>
    bool WriteMemoryRelative(HANDLE hProcess, DWORD_PTR address, T value)
    {
        DWORD_PTR relAddress = GetRelativeAddress(hProcess, address);

        SIZE_T written;
        BOOL bSuccess = WriteProcessMemory(hProcess, (LPVOID)relAddress, &value, sizeof(value), &written);

        // assert (ReadMemoryRelative(hProcess, address) == value && "WriteProcessMemory failed.");
        
        return bSuccess && written == sizeof(value);
    }

    template<typename T>
    bool WriteMemoryGlobal(HANDLE hProcess, DWORD_PTR address, T value)
    {
        SIZE_T written;
        BOOL bSuccess = WriteProcessMemory(hProcess, (LPVOID)relAddress, &value, sizeof(value), &written);

        return bSuccess && written == sizeof(value);
    }
};
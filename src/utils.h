/*
    Copyright (c) 2024 Piotr Stradowski. All rights reserved.
    Software distributed under the permissive MIT License.
*/

#pragma once

#include <windows.h>
#include <iostream>
#include <cassert>

namespace PatchingUtils
{
    class SmartProcessHandle;

	extern void EnableDebugPriv();
    extern size_t GetProcessCountByProcessName(const char* processName);
    extern DWORD GetProcessIdByProcessName(const char* processName);

    extern DWORD_PTR GetProcessBaseAddress(HANDLE hProcess);
    extern DWORD_PTR GetModuleBaseAddress(HANDLE hProcess, const char* moduleName);
    extern DWORD_PTR GetRelativeAddress(HANDLE hProcess, DWORD_PTR address);

    extern DWORD_PTR ReadMemoryRelative(HANDLE hProcess, DWORD_PTR address);
    extern DWORD_PTR ReadMemoryGlobal(HANDLE hProcess, DWORD_PTR address);
    
    template<typename T>
    bool WriteMemoryRelative(HANDLE hProcess, DWORD_PTR address, T value)
    {
        DWORD_PTR relativeAddress = GetRelativeAddress(hProcess, address);

        SIZE_T written;
        BOOL bSuccess = WriteProcessMemory(hProcess, (LPVOID)relativeAddress, &value, sizeof(value), &written);

        assert(bSuccess && "Writing process memory failed.");
        
        return bSuccess && written == sizeof(value);
    }

    template<typename T>
    bool WriteMemoryGlobal(HANDLE hProcess, DWORD_PTR globalAddress, T value)
    {
        SIZE_T written;
        BOOL bSuccess = WriteProcessMemory(hProcess, (LPVOID)globalAddress, &value, sizeof(value), &written);

        assert(bSuccess && "Writing process memory failed.");

        return bSuccess && written == sizeof(value);
    }
};
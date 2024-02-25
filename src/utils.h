#pragma once

#include <windows.h>
#include <iostream>

namespace PatchingUtils
{
    class SmartProcessHandle;

	extern void EnableDebugPriv();
    extern DWORD GetProcessIdByProcessName(const char* processName);
    extern DWORD_PTR GetProcessBaseAddress(HANDLE hProcess);
};
/*
    Copyright (c) 2024 Piotr Stradowski. All rights reserved.
    Software distributed under the permissive MIT License.
*/

#include "utils.h"
#include "smartProcessHandle.h"

#include <Psapi.h>
#include <iostream>
#include <TlHelp32.h>

namespace PatchingUtils
{
    // from: https://stackoverflow.com/questions/865152/how-can-i-get-a-process-handle-by-its-name-in-c
    void EnableDebugPriv()
    {
        HANDLE hToken;
        OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

        LUID luid;
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
        
        TOKEN_PRIVILEGES tkp;
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Luid = luid;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL);

        CloseHandle(hToken);
    }

    size_t GetProcessCountByProcessName(const char* processName)
    {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        
        size_t foundProcessCount = 0;
        if (Process32First(snapshot, &entry) == TRUE)
        {
            while (Process32Next(snapshot, &entry) == TRUE)
            {
                if (strcmp(entry.szExeFile, processName) == 0)
                {
                    // process found
                    foundProcessCount++;
                }
            }
        }

        return foundProcessCount;
    }

    DWORD GetProcessIdByProcessName(const char* processName)
    {
        // TODO: Return array of PID's
        
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        DWORD pid = NULL;

        size_t foundProcessCount = 0;
        if (Process32First(snapshot, &entry) == TRUE)
        {
            while (Process32Next(snapshot, &entry) == TRUE)
            {
                if (strcmp(entry.szExeFile, processName) == 0)
                {
                    // process found
                    foundProcessCount++;
                    pid = entry.th32ProcessID;

                    // try acquire handle for test
                    {
                        SmartProcessHandle hProcess = SmartProcessHandle(pid);
                        if (!hProcess.IsValid())
                        {
                            std::cout << "Failed to open process with name '" << processName << "' and PID " << pid << std::endl;
                            continue;
                        }
                    }
                    // test successful
                }
            }
        }
        
        return pid;
    }

    DWORD_PTR GetProcessBaseAddress(HANDLE hProcess)
    {
        DWORD_PTR   baseAddress = 0;
        HMODULE     hMods[1024];
        DWORD       cbNeeded;
        if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
        {
            baseAddress = (DWORD_PTR)hMods[0];
        }

        return baseAddress;
    }

    DWORD_PTR GetModuleBaseAddress(HANDLE hProcess, const char* moduleName)
    {
        HMODULE hModules[1024];
		DWORD cbNeeded;
        if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded))
        {
            for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
            {
				TCHAR szModName[MAX_PATH];
                if (GetModuleFileNameEx(hProcess, hModules[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
                {
					std::string modName = szModName;
                    if (modName.find(moduleName) != std::string::npos)
                    {
						return (DWORD_PTR)hModules[i];
					}
				}
			}
		}

		return 0;
    }

    DWORD_PTR GetRelativeAddress(HANDLE hProcess, DWORD_PTR address)
    {
        DWORD_PTR baseAddr = GetProcessBaseAddress(hProcess);
        return (DWORD_PTR) baseAddr + address;
    }

    DWORD_PTR ReadMemoryRelative(HANDLE hProcess, DWORD_PTR address)
    {
        DWORD_PTR relAddress = GetRelativeAddress(hProcess, address);

		DWORD_PTR value;
		SIZE_T bytesRead;
        ReadProcessMemory(hProcess, (LPCVOID) relAddress, &value, sizeof(value), &bytesRead);

		return value;
    }

    DWORD_PTR ReadMemoryGlobal(HANDLE hProcess, DWORD_PTR address)
    {
        DWORD_PTR value;
        SIZE_T bytesRead;
        ReadProcessMemory(hProcess, (LPCVOID)address, &value, sizeof(value), &bytesRead);

        return value;
    }
}

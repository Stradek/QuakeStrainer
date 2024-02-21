#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <cassert>
#include <psapi.h>

#define AMMO_ADDRESS 0x49BC88

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

struct SmartProcessHandle
{
    HANDLE hProcess;
    DWORD pid;
    DWORD_PTR baseAddress;

    SmartProcessHandle(HANDLE hProcess, DWORD pid) : hProcess(hProcess), pid(pid) 
    {
        if (hProcess)
        {
			baseAddress = GetProcessBaseAddress(hProcess);
		}
    }

    ~SmartProcessHandle()
    {
        if (hProcess)
        {
            if (CloseHandle(hProcess) == 0)
            {
                std::cout << "CloseHandle failed." << std::endl;
            }
        }
    }

    bool isValid()
    {
        return hProcess != nullptr;
    }

    HANDLE GetHandle()
    {
        assert(hProcess != nullptr);
        return hProcess;
    }

    DWORD_PTR GetRelativeAddr(DWORD_PTR address)
    {
		return baseAddress + address;
	}

};

SmartProcessHandle GetHandleToQuakeProcess()
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    HANDLE hProcess = NULL;
    DWORD pid = NULL;
    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (stricmp(entry.szExeFile, "quakespasm.exe") == 0)
            {
                // file process found
                pid = entry.th32ProcessID;

                hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
                if (hProcess == NULL) {
                    std::cout << "Failed to OpenProcess." << std::endl;
                    continue;
                }

                // recieved handle
                std::cout << "Process ID: " << pid << std::endl;
                break;
            }
        }
    }
    // failed to get handle
    return SmartProcessHandle(hProcess, pid);
}

int main() {
    EnableDebugPriv();

    SmartProcessHandle quakeProcHandle = GetHandleToQuakeProcess();
    if (!quakeProcHandle.isValid()) {
		std::cout << "Failed to get handle to Quake process." << std::endl;
		return 1;
	}
    DWORD_PTR baseAddress = GetProcessBaseAddress(quakeProcHandle.GetHandle());

    LPVOID ammoAddress = (LPVOID) quakeProcHandle.GetRelativeAddr(AMMO_ADDRESS); 
    DWORD value = 255;

    SIZE_T written;
    BOOL wpmResult = WriteProcessMemory(quakeProcHandle.GetHandle(), ammoAddress, &value, sizeof(value), &written);
    if (wpmResult == FALSE) {
        std::cout << "WriteProcessMemory failed." << std::endl;
    }
    else {
        std::cout << "Memory written successfully." << std::endl;
    }

    return 0;
}

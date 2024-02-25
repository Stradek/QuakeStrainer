#pragma once
#include "utils.h"
#include <cassert>

namespace PatchingUtils
{
    class SmartProcessHandle
    {
    public:
        SmartProcessHandle(DWORD pid) : pid(pid), baseAddress(0)
        {
            assert (pid != 0);

            hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
            if (hProcess == nullptr)
            {
				std::cout << "OpenProcess failed." << std::endl;
                return;
			}

            baseAddress = GetProcessBaseAddress(hProcess);
        }
        ~SmartProcessHandle()
        {
            assert (hProcess != nullptr);

            if (CloseHandle(hProcess) == 0)
            {
                std::cout << "CloseHandle failed." << std::endl;
            }
        }

        HANDLE operator *()
        {
			return hProcess;
		}

        bool IsValid();

        HANDLE GetHandle();
        DWORD_PTR GetBaseAddress();
        LPVOID GetRelativeAddr(DWORD_PTR address);

        template<typename T>
        BOOL WriteMemoryGlobal(LPVOID address, T value);
        template<typename T>
        BOOL WriteMemoryRelative(LPVOID address, T value);

    private:
        HANDLE hProcess;
        DWORD pid;
        DWORD_PTR baseAddress;
    };
}
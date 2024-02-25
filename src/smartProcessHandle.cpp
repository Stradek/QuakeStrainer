#include "smartProcessHandle.h"
#include "utils.h"

#include <iostream>
#include <cassert>

namespace PatchingUtils
{
    bool SmartProcessHandle::IsValid()
    {
        return hProcess != nullptr;
    }

    HANDLE SmartProcessHandle::GetHandle()
    {
        assert(hProcess != nullptr);
        return hProcess;
    }

    DWORD_PTR SmartProcessHandle::GetBaseAddress()
    {
        return baseAddress;
    }

    LPVOID SmartProcessHandle::GetRelativeAddr(DWORD_PTR address)
    {
        return  (LPVOID)(baseAddress + address);
    }

    template<typename T>
    BOOL SmartProcessHandle::WriteMemoryRelative(LPVOID address, T value)
    {
        LPVOID relAddress = GetRelativeAddr((DWORD_PTR)address);

        SIZE_T written;
        return WriteProcessMemory(hProcess, relAddress, &value, sizeof(value), &written);
    }

    template<typename T>
    BOOL SmartProcessHandle::WriteMemoryGlobal(LPVOID address, T value)
    {
        SIZE_T written;
        return WriteProcessMemory(hProcess, address, &value, sizeof(value), &written);
    }
}
/*
    Copyright (c) 2024 Piotr Stradowski. All rights reserved.
    Software distributed under the permissive MIT License.
*/

#include "smartProcessHandle.h"
#include "utils.h"

#include <iostream>
#include <cassert>

namespace PatchingUtils
{
    SmartProcessHandle::SmartProcessHandle(DWORD pid) : pid(pid)
    {
        assert(pid != 0);

        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (hProcess == NULL)
        {
            std::cout << "OpenProcess failed." << std::endl;
            return;
        }
    }

    SmartProcessHandle::~SmartProcessHandle()
    {
        if (hProcess == NULL)
        {
            return;
        }

        if (CloseHandle(hProcess) == 0)
        {
            std::cout << "CloseHandle failed." << std::endl;
        }
    }

    bool SmartProcessHandle::IsValid()
    {
        return hProcess != nullptr;
    }

    HANDLE SmartProcessHandle::GetHandle()
    {
        assert(hProcess != nullptr);
        return hProcess;
    }
}
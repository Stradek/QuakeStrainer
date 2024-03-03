/*
    Copyright (c) 2024 Piotr Stradowski. All rights reserved.
    Software distributed under the permissive MIT License.
*/

#pragma once
#include "utils.h"
#include <cassert>

namespace PatchingUtils
{
    class SmartProcessHandle
    {
    public:
        SmartProcessHandle(DWORD pid);
        ~SmartProcessHandle();

        HANDLE operator *()
        {
			return hProcess;
		}

        bool IsValid();
        HANDLE GetHandle();

    private:
        HANDLE hProcess;
        DWORD pid;
    };
}
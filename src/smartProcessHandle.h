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
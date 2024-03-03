/*
	Copyright (c) 2024 Piotr Stradowski. All rights reserved.
	Software distributed under the permissive MIT License.
*/

#pragma once
#include "utils.h"
#include "smartProcessHandle.h"

#include <windows.h>

class QuakeModdingAPI
{
private:
	DWORD pid;

	bool IsQuakeRunning();

	DWORD GetQuakeProcessId();
	DWORD_PTR GetQuakeProcessBaseAddress();
public:
	QuakeModdingAPI();
	~QuakeModdingAPI();

	bool Initialize();

	void SetAmmo(DWORD value);
};
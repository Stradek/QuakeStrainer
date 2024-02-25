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
	//DWORD_PTR GetQuakeModuleBaseAddress(DWORD pid, const char* moduleName);
public:
	QuakeModdingAPI();
	~QuakeModdingAPI();

	bool Initialize();


	void SetAmmo(DWORD value);
};
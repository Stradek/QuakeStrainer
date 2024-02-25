#include "quakePatchingApi.h"
#include "utils.h"

QuakeModdingAPI::QuakeModdingAPI()
{
}

QuakeModdingAPI::~QuakeModdingAPI()
{

}

bool QuakeModdingAPI::Initialize()
{
	pid = GetQuakeProcessId();
	if (pid == 0)
	{
		std::cout << "Failed to acquire Quake process ID." << std::endl;
	}
	else
	{
		std::cout << "Quake is running with process ID: " << pid << "." << std::endl;
	}

	return pid != 0;
}

bool QuakeModdingAPI::IsQuakeRunning()
{
	return GetQuakeProcessId() != 0;
}

DWORD QuakeModdingAPI::GetQuakeProcessId()
{
	const char* processName = "quakespasm.exe";
	DWORD pid = PatchingUtils::GetProcessIdByProcessName(processName);
	return pid;
}

DWORD_PTR QuakeModdingAPI::GetQuakeProcessBaseAddress()
{
	DWORD_PTR baseAddr;
	{
		PatchingUtils::SmartProcessHandle hProcess = PatchingUtils::SmartProcessHandle(GetQuakeProcessId());
		return PatchingUtils::GetProcessBaseAddress(hProcess.GetHandle());
	}
}

//DWORD_PTR QuakePatchingAPI::GetQuakeModuleBaseAddress(DWORD pid, const char* moduleName)
//{
//	// placeholder for proper code
//}

void QuakeModdingAPI::SetAmmo(DWORD value)
{
	std::cout << "Setting ammo not yet implemented." << std::endl;
}

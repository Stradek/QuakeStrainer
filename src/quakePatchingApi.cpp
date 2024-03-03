#include "quakePatchingApi.h"
#include "utils.h"

#define AMMO_ADDRESS_1 0x49BC88
#define AMMO_ADDRESS_2 0x49BC94
#define AMMO_PTR_ADDRESS_1 0x4F1D98
#define AMMO_PTR_ADDRESS_1_OFFSET_1 0x88
#define AMMO_PTR_ADDRESS_1_OFFSET_2 0x8C

QuakeModdingAPI::QuakeModdingAPI() : pid(0)
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
	PatchingUtils::SmartProcessHandle hProcess = PatchingUtils::SmartProcessHandle(GetQuakeProcessId());
	return PatchingUtils::GetProcessBaseAddress(hProcess.GetHandle());
}

void QuakeModdingAPI::SetAmmo(DWORD value)
{
	PatchingUtils::SmartProcessHandle hProcess = PatchingUtils::SmartProcessHandle(GetQuakeProcessId());

	DWORD_PTR ammoPtrAddr = PatchingUtils::ReadMemoryRelative(hProcess.GetHandle(), AMMO_PTR_ADDRESS_1);
	if (ammoPtrAddr == 0)
	{
		std::cout << "Failed to read ammo pointer address." << std::endl;
		return;
	}

	bool bSuccess = PatchingUtils::WriteMemoryRelative<DWORD>(hProcess.GetHandle(), (DWORD_PTR)AMMO_ADDRESS_1, value) &&
		PatchingUtils::WriteMemoryRelative<DWORD>(hProcess.GetHandle(), (DWORD_PTR)AMMO_ADDRESS_2, value) &&
		PatchingUtils::WriteMemoryGlobal<FLOAT>(hProcess.GetHandle(), ammoPtrAddr + AMMO_PTR_ADDRESS_1_OFFSET_1, (FLOAT) value) &&
		PatchingUtils::WriteMemoryGlobal<FLOAT>(hProcess.GetHandle(), ammoPtrAddr + AMMO_PTR_ADDRESS_1_OFFSET_2, (FLOAT) value);
	
	if (!bSuccess)
	{
		std::cout << "Writing new ammo value failed." << std::endl;
		return;
	}

	std::cout << "Ammo set to " << value << "." << std::endl;
}

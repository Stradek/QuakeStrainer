/*
	Copyright (c) 2024 Piotr Stradowski. All rights reserved.
	Software distributed under the permissive MIT License.
*/

#include "quakePatchingApi.h"

#include "smartProcessHandle.h"
#include "utils.h"

#include "jobScheduler.h"

#define QuakeProcessName "quakespasm.exe"

enum QuakeMemAddresses : DWORD_PTR
{
	AMMO_ADDRESS_1 = 0x49BC88,
	AMMO_ADDRESS_2 = 0x49BC94,
	AMMO_PTR_ADDRESS_1 = 0x4F1D98,
	AMMO_PTR_ADDRESS_1_OFFSET_1 = 0x88,
	AMMO_PTR_ADDRESS_1_OFFSET_2 = 0x8C
};

enum JobSystemSettings : DWORD
{
	THREAD_COUNT = 2,
	BACKGROUND_THREAD_UPDATE_INTERVAL = 100
};

QuakeModdingApi::QuakeModdingApi()
{
	JobScheduler::createInstance(THREAD_COUNT);
}

QuakeModdingApi::~QuakeModdingApi()
{
	JobScheduler::destroyInstance();
}

bool QuakeModdingApi::CheckQuakeInstanceRunning()
{
	size_t quakeInstancesRunningCount = QuakeModdingApi::GetQuakeProcessCount();
	if (quakeInstancesRunningCount == 1)
	{
		return true;
	}
	else
	{
		if (quakeInstancesRunningCount == 0)
		{
			std::cout << "There is no Quake game instance running." << std::endl;
		}
		else if (quakeInstancesRunningCount > 1)
		{
			std::cout << "There is more than 1 Quake game instance running. "
				   "Make sure you have only one game running when using this trainer." << std::endl;
		}
		return false;
	}
}

bool QuakeModdingApi::IsQuakeInstanceRunning()
{
	return GetQuakeProcessCount() == 1;
}

bool InitQuakeStateThread()
{
	QuakeModdingApi* QuakeModdingApiInstancePtr = &this; 
	JobScheduler::enqueueRepeatingJob([] => {
		UpdateQuakeState();
		std::this_thread::sleep_for(std::chrono::milliseconds(BACKGROUND_THREAD_UPDATE_INTERVAL));
		return JobStatus::Running;
	});
}


bool QuakeModdingApi::Initialize()
{
	m_pid = GetQuakeProcessId();
	UpdateQuakeState();
	
	if (m_state == QuakeState::NotRunning)
	{
		std::cout << "Quake is not running." << std::endl;
		return false;
	}

	return true;
}

bool QuakeModdingApi::IsPlayerSpawned()
{
	if (!CheckQuakeInstanceRunning())
	{
		return false;
	}
	
	PatchingUtils::SmartProcessHandle hProcess = PatchingUtils::SmartProcessHandle(m_pid);
	const DWORD_PTR ammoPtrAddr = PatchingUtils::ReadMemoryRelative(hProcess.GetHandle(), AMMO_PTR_ADDRESS_1);
	if (ammoPtrAddr == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool QuakeModdingApi::CheckPlayerSpawned()
{
	if (!IsPlayerSpawned())
	{
		std::cout << "Failed to find player instance. Make sure player is spawned." << std::endl;
		return false;
	}

	return true;
}

size_t QuakeModdingApi::GetQuakeProcessCount()
{
	return PatchingUtils::GetProcessCountByProcessName(QuakeProcessName);
}

DWORD QuakeModdingApi::GetQuakeProcessId()
{
	return PatchingUtils::GetProcessIdByProcessName(QuakeProcessName);
}

DWORD_PTR QuakeModdingApi::GetQuakeProcessBaseAddress()
{
	PatchingUtils::SmartProcessHandle hProcess = PatchingUtils::SmartProcessHandle(GetQuakeProcessId());
	return PatchingUtils::GetProcessBaseAddress(hProcess.GetHandle());
}

void QuakeModdingApi::UpdateQuakeState()
{
	// I will leave it for now like this, until I figure out other possible states
	
	if (!IsQuakeInstanceRunning())
	{
		SetState(QuakeState::NotRunning);
	}
	else if(IsPlayerSpawned())
	{
		SetState(QuakeState::PlayerSpawned);
	}
	else
	{
		SetState(QuakeState::Running);
	}
}

bool QuakeModdingApi::SetAmmo(const unsigned int value)
{
	if (m_state != QuakeState::PlayerSpawned)
	{
		return false;
	}

	PatchingUtils::SmartProcessHandle hProcess = PatchingUtils::SmartProcessHandle(GetQuakeProcessId());

	const DWORD_PTR ammoPtrAddr = PatchingUtils::ReadMemoryRelative(hProcess.GetHandle(), AMMO_PTR_ADDRESS_1);
	if (ammoPtrAddr == 0)
	{
		std::cout << "Failed to read ammo pointer address. Make sure player character is spawned." << std::endl;
		return false;
	}

	const bool bSuccess = PatchingUtils::WriteMemoryRelative<DWORD>(hProcess.GetHandle(), (DWORD_PTR)AMMO_ADDRESS_1, value) &&
		PatchingUtils::WriteMemoryRelative<DWORD>(hProcess.GetHandle(), (DWORD_PTR)AMMO_ADDRESS_2, value) &&
		PatchingUtils::WriteMemoryGlobal<FLOAT>(hProcess.GetHandle(), ammoPtrAddr + AMMO_PTR_ADDRESS_1_OFFSET_1, (FLOAT) value) &&
		PatchingUtils::WriteMemoryGlobal<FLOAT>(hProcess.GetHandle(), ammoPtrAddr + AMMO_PTR_ADDRESS_1_OFFSET_2, (FLOAT) value);
	
	if (!bSuccess)
	{
		std::cout << "Writing new ammo value failed." << std::endl;
		return false;
	}
	return true;
}

bool QuakeModdingApi::UpdateUnlimitedAmmo()
{
	if (m_state != QuakeState::PlayerSpawned)
	{
		return false;
	}
	
	const bool bSuccess = SetAmmo(999);
	if (!bSuccess)
	{
		std::cout << "Failed to set ammo." << std::endl;
		return false;
	}
	return true;
}

bool QuakeModdingApi::ToggleOnUnlimitedAmmo()
{
	if (m_state != QuakeState::PlayerSpawned)
	{
		return false;
	}

	JobScheduler::getInstance()->enqueueRepeatingJob([this]()
		{
			if (m_state != QuakeState::PlayerSpawned)
			{
				return JobStatus::Finished;
			}
		
			const bool bSuccess = UpdateUnlimitedAmmo();
			if (!bSuccess)
			{
				std::cout << "Failed to update unlimited ammo." << std::endl;
				return JobStatus::Failed;
			}
			
			std::this_thread::sleep_for(std::chrono::milliseconds(BACKGROUND_THREAD_UPDATE_INTERVAL));
			return JobStatus::Running;
		});

	std::cout << "Unlimited ammo enabled." << std::endl;
	return true;
}

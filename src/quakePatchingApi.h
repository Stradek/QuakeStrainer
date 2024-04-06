/*
	Copyright (c) 2024 Piotr Stradowski. All rights reserved.
	Software distributed under the permissive MIT License.
*/

#pragma once

#include <windows.h>

class JobScheduler;

enum class QuakeState
{
	Running,
	PlayerSpawned,
	NotRunning
};

class QuakeModdingApi
{
public:
	QuakeModdingApi();
	~QuakeModdingApi();

	static size_t GetQuakeProcessCount();
	
	static bool IsOneQuakeInstanceRunning();
	static bool CheckOneQuakeInstanceRunning();

	bool IsPlayerSpawned();
	bool CheckPlayerSpawned();
	
	bool Initialize();

	bool SetAmmo(unsigned int value);
	bool ToggleOnUnlimitedAmmo();

private:
	DWORD m_pid = 0;
	QuakeState m_state = QuakeState::NotRunning;

private:
	static DWORD GetQuakeProcessId();
	static DWORD_PTR GetQuakeProcessBaseAddress();

	void UpdateQuakeState();
	void SetState(QuakeState state) {m_state = state;}

	friend class JobScheduler;
	bool UpdateUnlimitedAmmo();
};
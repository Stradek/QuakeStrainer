/*
    Copyright (c) 2024 Piotr Stradowski. All rights reserved.
    Software distributed under the permissive MIT License.
*/

#include "quakePatchingApi.h"
#include "utils.h"

#include <iostream>
#include <thread>

#define THREAD_WAIT_INVERVAL 100

void WaitForOneQuakeGameRunning()
{
    QuakeModdingApi::CheckOneQuakeInstanceRunning();

    while (QuakeModdingApi::GetQuakeProcessCount() != 1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_WAIT_INVERVAL));
    }
    std::cout << "One Quake game instance found." << std::endl;
}

void InitializeQuakeModdingAPI(QuakeModdingApi& quakeModdingApi)
{
    if (!quakeModdingApi.Initialize())
    {
        std::cout << "Initializing QuakeModdingAPI failed." << std::endl;
        return;
    }
    std::cout << "QuakeModdingAPI initialized." << std::endl;
}

bool WaitForPlayerToSpawn(QuakeModdingApi& quakeModdingApi)
{
    std::cout << "Waiting for player to spawn..." << std::endl;
    while (true)
    {
        if (!QuakeModdingApi::IsOneQuakeInstanceRunning())
        {
            return false;
        }
        
        if (quakeModdingApi.IsPlayerSpawned())
        {
            break;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_WAIT_INVERVAL));
    }
    std::cout << "Found spawned player." << std::endl;
    return true;
}

int main() {
    PatchingUtils::EnableDebugPriv();

    {
        QuakeModdingApi quakeModdingApi = QuakeModdingApi();
        
        bool bCanUseTrainer = false;
        while (!bCanUseTrainer)
        {
            WaitForOneQuakeGameRunning();
            InitializeQuakeModdingAPI(quakeModdingApi);
            bCanUseTrainer = WaitForPlayerToSpawn(quakeModdingApi);
        }
        
        if (bCanUseTrainer)
        {
            quakeModdingApi.ToggleOnUnlimitedAmmo();
        }

        std::cout << "Press Enter to close trainer..." << std::endl;
        std::cin.get();
    }

    return 0;
}

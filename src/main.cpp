/*
    Copyright (c) 2024 Piotr Stradowski. All rights reserved.
    Software distributed under the permissive MIT License.
*/

#include "utils.h"

#include <iostream>
#include "quakePatchingApi.h"

int main() {
    PatchingUtils::EnableDebugPriv();

    QuakeModdingAPI quakeProcHandle = QuakeModdingAPI();
    if (!quakeProcHandle.Initialize())
    {
        std::cout << "Initializing QuakeModdingAPI failed." << std::endl;
		return 1;
    }

    quakeProcHandle.SetAmmo(111);

    return 0;
}

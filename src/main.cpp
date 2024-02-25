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

#include "Windows.h"
#include "source/me/sleepyfish/autoclicker/Utils.h"
#include "source/me/sleepyfish/autoclicker/ClickUtils.h"

ClickUtils* rightClickUtils{};
ClickUtils* leftClickUtils{};

bool running = false;

void Init(HMODULE mod) {

    // initialize click utils for left and right clicks
    {
        rightClickUtils = new ClickUtils(1);
        leftClickUtils = new ClickUtils(0);

        rightClickUtils->init();
        leftClickUtils->init();
    }

    // print that the dll is injected if console mode is enabled
    Utils::messageBox(std::string("AutoClicker Injected\n\n\nHold Left -> Left AutoClick\nHold Right -> Right AutoClick\n"
        "\nHold backspace to Detach\n\n\nInjected into: " + Utils::getCurrentProcessPathName()).c_str());

    running = true;

    // main loop
    while (running) {

        // check if ingame focus
        if (Utils::hasIngameFocus()) {

            // update click utils if ur holding right mouse button down
            if (Utils::holdingMouseRight())
                rightClickUtils->update(15, 18);

            // update click utils if ur holding left mouse button down
            if (Utils::holdingMouseLeft())
                leftClickUtils->update(10, 14);

            // exit the whole main loop if ur holding backspace
            if (GetAsyncKeyState(VK_BACK) & 0x8000) {
                running = false;
                continue;
            }
        }

        // sleep for 0.20 seconds
        Sleep(20L);
    }

    Utils::messageBox("AutoClicker Detached");

    // free click utils for memory management
    delete rightClickUtils;
    delete leftClickUtils;

    // free the dll / like detaching
    FreeLibrary(mod);
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {

    // checks if the dll action is attach
    if (dwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        // create thread for the main loop passing argument hModule to the init function
        HANDLE hThread = CreateThread (
    nullptr, 0, (LPTHREAD_START_ROUTINE) (Init), hModule, 0, nullptr
        );

        // close the thread
        if (hThread != nullptr)
            CloseHandle(hThread);
    }

    return true;
}
#include "global_macros.hpp"
#include "code.hpp"

#include <iostream>
#include <Windows.h>
#include <string>

constexpr const char* GAME_MODULE = "cs2.exe";

static void __stdcall DLL_Init(HMODULE hModule);
static void DLL_GameplayLoop();

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DLL_Init, hModule, NULL, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void __stdcall DLL_Init(HMODULE hModule)
{

    auto _module = GetModuleHandleA(GAME_MODULE);

    if (!_module) {
        const auto str = std::format("Can't find module \"{}\"", GAME_MODULE);
        MessageBoxA(NULL, "Error!", str.c_str(), MB_ICONERROR);
        return;
    }

    AllocConsole();
    FILE* _con;
    freopen_s(&_con, "CONOUT$", "w", stdout);

    DLL_GameplayLoop();

    if (_con)
        fclose(_con );

    FreeConsole();
    FreeLibraryAndExitThread((HMODULE)hModule, 0);

}


void DLL_GameplayLoop()
{
    __init();

    while (true)
    {
        if (KeyPressed(VK_HOME))
            break;

        __main();
    }

    std::cout << "\nGoodbye!!\n";
    Sleep(500);
    return;
}
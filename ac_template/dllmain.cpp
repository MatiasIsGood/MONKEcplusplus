// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
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
    iw3sp = (DWORD)GetModuleHandleA(GAME_MODULE);

    if (!iw3sp) {
        char err[128];

        sprintf_s(err, "Can't find module \"%s\"", GAME_MODULE);
        MessageBoxA(NULL, "Error!", err, MB_ICONERROR);

        return;
    }

    AllocConsole();
    freopen_s(&_con, "CONOUT$", "w", stdout);

    DLL_GameplayLoop();
    if (_con)
        fclose(_con );
    FreeConsole();

    FreeLibraryAndExitThread(hModule, 0);

}


void DLL_GameplayLoop()
{
    //std::cout << "yea\n";

    while (true)
    {
        __main();

        if (KeyPressed(VK_HOME))
            break;

    }
    std::cout << "Goodbye!!\n";
    
    

    Sleep(500);
    return;
}
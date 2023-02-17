#pragma once

#ifndef _source__
#define _source__


#include "pch.h"

constexpr const char* GAME_MODULE = "iw3mp.exe";

inline DWORD iw3sp;

inline FILE* _con;

void __stdcall DLL_Init(HMODULE hModule);
void DLL_GameplayLoop();

#endif
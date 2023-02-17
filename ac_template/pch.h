// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define KeyPressed(x)	(GetAsyncKeyState(x) & 1)
#define KeyHeld(x)		(GetAsyncKeyState(x) < 0)

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//DIRECTX
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9core.h>

// add headers that you want to pre-compile here
#include "framework.h"

#include "cg_local.h"
#include "cg_offsets.h" 

#include "source.hpp"
#include "code.hpp"

#endif //PCH_H

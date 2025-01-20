#pragma once

#define KeyPressed(x)	(GetAsyncKeyState(x) & 1)
#define KeyHeld(x)		(GetAsyncKeyState(x) < 0)


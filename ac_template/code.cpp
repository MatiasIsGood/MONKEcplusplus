#include "global_macros.hpp"
#include "cs2/interface.hpp"
#include "cs2/interfaces/icvar.hpp"
#include "cs2/engine/cmd.hpp"

#include <Windows.h>
#include <iostream>
#include <ranges>

//kerran kun ohjelma alkaa
void __init()
{
	CStaticInterfaces::Init();
	CStaticInterfaces::InitNested();

}

//loop
void __main()
{
	if (KeyPressed(VK_NUMPAD0)) {

		for (int i = 0; i < 100; i++) {
			Con_SendCommand("say hello");
			Sleep(100);
		}
	}

	Sleep(3);
}
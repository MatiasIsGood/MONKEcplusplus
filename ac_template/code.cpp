#include "global_macros.hpp"
#include "cs2/interface.hpp"
#include "cs2/interfaces/icvar.hpp"
#include "cs2/engine/cmd.hpp"
#include "cs2/client/vgui_fpspanel.hpp"

#include "utils/hook.hpp"

#include <Windows.h>
#include <iostream>
#include <string>
#include <ranges>

//client.dll+1A6888C

size_t client = (size_t)GetModuleHandleA("client.dll");

//kerran kun ohjelma alkaa
void __init()
{
	CStaticInterfaces::Init();
	CStaticInterfaces::InitNested();

    if (!hooktable::initialize())
        return;

	HookVGUI();

}

void bunnyhop() {

	size_t inAirAddrs = (client + 0x1A6888C);
	bool inAir = *(bool*)inAirAddrs;

	if (inAir) {
		Con_SendCommand("-jump");
	}
	else {
		Con_SendCommand("+jump");
	}

}

//loop
void __main()
{

	CConVar *dev = ICvar::Find("developer");

	if (dev->m_oValue.m_i32Value == 1) {
		bunnyhop();
	}


	if (KeyPressed(VK_INSERT)) {

		//for (int i = 0; i < 100; i++) {
		//	Con_SendCommand("say hello");
		//	Sleep(100);
		//}
	}

	Sleep(3);
}

void __exit() {
	hooktable::for_each([](std::unique_ptr<hookbase>& ptr) { ptr->release(); });
	hooktable::release();
}
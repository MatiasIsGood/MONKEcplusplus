#include "global_macros.hpp"
#include "cs2/interface.hpp"
#include "cs2/interfaces/icvar.hpp"

#include <Windows.h>
#include <iostream>
#include <ranges>

//kerran kun ohjelma alkaa
void __init()
{
	CStaticInterfaces::Init();
	CStaticInterfaces::InitNested();

}

void destroyGame() {
	ConVarElem* vars = ICvar::GetAllCvars();
	unsigned short totalVariables = ICvar::GetCount();

	for (int i = 0; i < totalVariables; i++) {
		CConVar* var = vars[i].element;
		if (var != nullptr && (var->m_eType == EConVarType_Int32)) {
			var->m_oValue.m_i32Value += 1;
		}
	}
}

void removeFlag(CConVar* var, int64_t flag) {
	var->m_iFlags &= ~flag;
}

void PrintCheats()
{
	ConVarElem* vars = ICvar::GetAllCvars();
	unsigned short totalVariables = ICvar::GetCount();
	
	for (int i = 0; i < totalVariables; i++) {
		CConVar* var = vars[i].element;
		if (var != nullptr && (var->m_iFlags & FCVAR_HIDDEN) != 0) {
			std::cout << var->m_sName << "\n";
			removeFlag(var, FCVAR_CHEAT);
			removeFlag(var, FCVAR_HIDDEN);
		}
	}

}

//loop
void __main()
{
	if (KeyPressed(VK_INSERT)) {
		
		destroyGame();
	}

	Sleep(3);
}
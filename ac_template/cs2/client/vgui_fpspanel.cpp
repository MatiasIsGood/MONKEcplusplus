#include "utils/engine.hpp"
#include "utils/hook.hpp"

#include <iostream>

using namespace std::string_literals;

static scan_data FPSPanel_PaintPattern{
    "\x48\x89\x54\x24\x10\x55\x41\x55\x41\x56\x48\x8D\xAC\x24\x70\xED\xFF\xFF\xB8\x90\x13\x00\x00\xE8\x54\x82\xA1\x00\x48\x2B\xE0"s,
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"s };


void __fastcall CFPSPanel_Paint(void* _this, std::int64_t a);

void HookVGUI()
{
    auto callable = Engine::Tools::FindPatternFromProcess("cs2.exe", "client.dll", FPSPanel_PaintPattern);

	if (!callable) {
		throw std::exception("HookVGUI() -> looks like the pattern has changed :(\n");
	}

	std::cout << "hooking to: " << std::hex << callable << '\n';

	hooktable::preserver<void*, std::int64_t>("CFPSPanel_Paint", callable, CFPSPanel_Paint);

}

void __fastcall CFPSPanel_Paint(void* _this, std::int64_t a)
{

	std::cout << "hello!!!!!!!!!\n";

	return;

}

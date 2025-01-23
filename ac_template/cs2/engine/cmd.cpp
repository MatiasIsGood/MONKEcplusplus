#include "cmd.hpp"
#include "utils/engine.hpp"

using namespace std::string_literals;

static scan_data Cbuf_AddTextPattern{ 
	"\x48\x8B\x0D\x91\x81\x3B\x00\x45\x33\xC9\x4C\x8B\xC2\x48\x8B\x01\x41\x8D\x51\x05\x48\xFF\xA0\xC8\x00\x00\x00"s, 
	"xxx????xxxxxxxxxxxxxxxxxxxxx"s };

void Cbuf_AddText(const char* text)
{
	static std::uintptr_t callable{};

	if (!callable) {
		callable = Engine::Tools::FindPatternFromProcess("cs2.exe", "engine2.dll", Cbuf_AddTextPattern);
	}

	if (!callable) {
		throw std::exception("Cbuf_AddText() -> looks like the pattern has changed :(\n");
	}

	auto func = ((void(*)(std::int64_t, const char*))(callable));

	func(0, text);
}

void Con_SendCommand(const std::string& cmd)
{
	if (cmd.empty())
		return;

	if (cmd.back() != '\n') {
		const auto newStr = cmd + '\n';
		return Cbuf_AddText(newStr.c_str());
	}

	return Cbuf_AddText(cmd.c_str());
}

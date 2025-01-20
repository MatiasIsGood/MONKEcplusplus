#pragma once
#include <type_traits>
#include <string>
#include <Windows.h>
#include <optional>

template<typename T>
concept MemoryAddress_t = std::is_integral_v<T> || std::is_pointer_v<T>;

namespace Engine
{
	template<typename Return, MemoryAddress_t Mem, typename ... Args>
	constexpr inline Return call(const Mem address, Args... args)
	{
		return (reinterpret_cast<Return(*)(Args...)>(address))(args...);
	}
}

using namespace std::string_literals;

namespace Engine::Tools
{

	template<MemoryAddress_t Mem>
	[[nodiscard]] inline bool write_bytes(Mem dst, const std::string& bytes) {
		DWORD oldProtect = {};
		const auto size = bytes.length();

		if (!VirtualProtect(reinterpret_cast<LPVOID>(dst), size, PAGE_EXECUTE_READWRITE, &oldProtect))
			return false;

		if (!memcpy_s(reinterpret_cast<LPVOID>(dst), size, bytes.c_str(), size))
			return false;

		if (!VirtualProtect(reinterpret_cast<LPVOID>(dst), size, oldProtect, &oldProtect))
			return false;

		return true;
	}

	template<MemoryAddress_t Mem>
	[[nodiscard]] bool nop(Mem dst) {
		return write_bytes(dst, "\x90\x90\x90\x90\x90"s);
	}
	
}

namespace Engine::Tools
{
	struct scan_data_t
	{
		constexpr scan_data_t() = default;
		constexpr scan_data_t(const std::uintptr_t size) {
			pattern.resize(size);
			mask.resize(size);
		}
		constexpr scan_data_t(const std::string& p, const std::string& m) : pattern(p), mask(m){}

		std::string pattern;
		std::string mask;
	};

	// - does a compile time evaluation of a raw pattern and returns an object containing the pattern and mask
	[[nodiscard]] extern constexpr scan_data_t ParsePattern(const char* raw_data);

	[[nodiscard]] extern char* ScanSection(const scan_data_t& data, const char* begin, const uintptr_t section_size);
	[[nodiscard]] uintptr_t FindPattern(HANDLE proc, std::uintptr_t begin, std::uintptr_t end, const scan_data_t& data);

	[[nodiscard]] uintptr_t FindPatternFromProcess(const std::string& proc, const std::optional<std::string>& dll, const scan_data_t& data);

}

using scan_data = Engine::Tools::scan_data_t;

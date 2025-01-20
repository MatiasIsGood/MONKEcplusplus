#pragma once
#include <unordered_map>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>

namespace Engine
{
	struct process_t;
	struct module_t : public MODULEENTRY32
	{
		module_t() = default;
		module_t(const process_t& proc, const std::string_view& modname) : m_pProcess(&proc) {
			m_bValid = Get(modname);
		}

		[[nodiscard]] constexpr bool IsValid() const noexcept { return m_bValid; }

		std::uintptr_t begin = 0;
		std::uintptr_t end = 0;
	private:
		[[nodiscard]] bool Get(const std::string_view& modname);

		const process_t* m_pProcess;
		bool m_bValid = false;

	};

	using module_map = std::unordered_map<std::string, module_t>;
	struct process_t : public PROCESSENTRY32
	{
		process_t() = default;
		process_t(const std::string_view& process);
		~process_t();
		
		[[nodiscard]] HANDLE Open(DWORD flags = PROCESS_ALL_ACCESS);
		[[maybe_unused]] bool Close() const;

		[[nodiscard]] module_t GetModule(const std::string_view& modname) const;
		[[maybe_unused]] module_t* LoadModule(const std::string& modname);

		[[nodiscard]] constexpr HANDLE GetHandle() const noexcept { return m_pHandle; }
		void SetHandle(HANDLE newHandle);
		[[nodiscard]] bool IsValid() const;
		[[nodiscard]] bool IsActive() const;

		std::size_t begin{};
		std::size_t end{};

	private:
		[[maybe_unused]] bool Get(const std::string_view& proc);
		DWORD GetExitCode() const;

		HANDLE m_pHandle = {};
		HMODULE m_pModuleBase = {};
		module_map m_oModules;
		MODULEINFO m_oModuleInfo = {};
	};
}

#include "process.hpp"

#include <locale>
inline std::wstring ToWide(const std::string& string) {

    std::locale loc;

    const auto str = string.data();

    auto len = strlen(str);

    std::wstring wide_str;
    wide_str.resize(len);

    std::use_facet<std::ctype<wchar_t>>(loc).widen(str, str + len, wide_str.data());

    return wide_str;
}

inline std::string ToNarrow(const std::wstring& wide_str) {
    std::locale loc;

    const auto str = wide_str.data();

    auto len = wide_str.length();

    std::string narrow_str;
    narrow_str.resize(len);

    std::use_facet<std::ctype<wchar_t>>(loc).narrow(str, str + len, '?', &narrow_str[0]);

    return narrow_str;
}

Engine::process_t::process_t(const std::string_view& process) {
    Get(process);
}
Engine::process_t::~process_t() = default;

void GetModuleAddresses(HMODULE hModule, std::size_t& baseAddress, std::size_t& endAddress) {

    MODULEINFO moduleInfo;
    if (GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo))) {
        baseAddress = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
        endAddress = baseAddress + moduleInfo.SizeOfImage;
    }
}

bool Engine::process_t::Get(const std::string_view& proc_)
{
    auto& procEntry = *this;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (hSnapshot)
    {
        procEntry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnapshot, &procEntry))
        {
            do
            {
                auto proc = proc_.data();

#ifdef UNICODE
                auto str = std::wstring(procEntry.szExeFile);
                auto bleh = ToNarrow(str);
                auto exeFile = bleh.data();
#else
                auto exeFile = procEntry.szExeFile;
#endif
                if (!strcmp(exeFile, proc))
                {
                    m_pModuleBase = GetModuleHandleA(proc);
                    m_pHandle = hSnapshot;
                    GetModuleAddresses(m_pModuleBase, begin, end);
                    return true;
                }
            } while (Process32Next(hSnapshot, &procEntry));
        }
        CloseHandle(hSnapshot);
    }

    return false;
}
[[nodiscard]] HANDLE Engine::process_t::Open(DWORD flags)
{
    if (!IsValid())
        return 0;

    CloseHandle(m_pHandle);

    m_pHandle = OpenProcess(flags, NULL, th32ProcessID);
    return m_pHandle;
}
bool Engine::process_t::Close() const
{
    if (!IsValid())
        return false;

    return CloseHandle(m_pHandle);
}

bool Engine::module_t::Get(const std::string_view& modname)
{
    if (!m_pProcess)
        return false;

    auto& modEntry = *this;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_pProcess->th32ProcessID);

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        modEntry.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(hSnapshot, &modEntry))
        {
            do
            {
                auto _str = modname.data();

#ifdef UNICODE
                auto str = std::wstring(modEntry.szModule);
                auto bleh = ToNarrow(str);
                auto szModule = bleh.data();
#else
                auto szModule = modEntry.szModule;
#endif

                if (!lstrcmpA(szModule, _str))
                {
                    CloseHandle(hSnapshot);

                    begin = reinterpret_cast<std::uintptr_t>(modBaseAddr);
                    end = begin + static_cast<std::uintptr_t>(modBaseSize);

                    return true;
                }
            } while (Module32Next(hSnapshot, &modEntry));
        }
        CloseHandle(hSnapshot);
    }
    return false;
}
Engine::module_t* Engine::process_t::LoadModule(const std::string& modname) {

    auto mod = GetModule(modname);
    if (mod.IsValid()) {

        m_oModules[modname] = std::move(mod);
        try {
            return &m_oModules.at(modname);
        }
        catch (...) {
            return nullptr;
        }
    }

    return nullptr;

}

void Engine::process_t::SetHandle(HANDLE newHandle) 
{ 
    if (IsActive())
        CloseHandle(m_pHandle);

    m_pHandle = newHandle; 
}

Engine::module_t Engine::process_t::GetModule(const std::string_view& modname) const 
{
    return module_t(*this, modname); 
}
bool Engine::process_t::IsValid() const
{
    if (!m_pHandle)
        return false;

    //return m_pHandle;
    DWORD code{};
    return GetHandleInformation(m_pHandle, &code);

}
DWORD Engine::process_t::GetExitCode() const
{
    DWORD code = {};
    if (GetExitCodeProcess(m_pHandle, &code)) {
        return code;
    }
    return NULL;
}
[[nodiscard]] bool Engine::process_t::IsActive() const
{
    return GetExitCode() == STILL_ACTIVE;
}



#include "engine.hpp"
#include "process.hpp"

template <typename T>
concept integral = std::is_integral_v<T>;

template<integral T>
static constexpr T hex2int(const char hexChar) {
    if (hexChar >= '0' && hexChar <= '9')
        return hexChar - '0';
    else if (hexChar >= 'A' && hexChar <= 'F')
        return hexChar - 'A' + 10;
    else if (hexChar >= 'a' && hexChar <= 'f')
        return hexChar - 'a' + 10;

    return -1;
}

template<integral T>
static constexpr T hex2T(const char* hexString) {
    unsigned int result = 0;
    while (*hexString != '\0') {
        T value = hex2int<T>(*hexString++);
        if (value == (T)-1)
            break;
        result = (result << 4) + value;
    }
    return T(result);
}

constexpr scan_data Engine::Tools::ParsePattern(const char* raw)
{
    constexpr auto __strlen = [](const auto* data) -> std::uintptr_t
        {
            std::uintptr_t size = 0ull;
            while (data[size] != '\0') { size++; }
            return size;
        };

    const auto size = __strlen(raw);
    auto pattern_size = (size + 1u) / 3u;

    scan_data data(pattern_size);

    for (auto i = uintptr_t(0); i < size; ++i) {

        if (raw[i] == ' ')
        {
            continue;
        }

        else if (raw[i] == '?') {
            data.mask[(i + 1u) / 3u] = '?';
            i += 2;
        }

        else {
            const auto byte = hex2T<char>(&raw[i]);
            data.pattern[(i + 1u) / 3u] = byte;
            data.mask[(i + 1u) / 3u] = 'x';
            i += 2;
        }

    }

    data.pattern[pattern_size] = '\0';
    data.mask[pattern_size] = '\0';
    return data;
}

char* Engine::Tools::ScanSection(const scan_data& data, const char* begin, const uintptr_t section_size)
{
    auto len = data.pattern.size();
    auto& pattern = data.pattern;
    auto& mask = data.mask;

    for (auto i = uintptr_t(0); i < section_size - len; i++)
    {
        bool found = true;
        for (auto j = 0u; j < len; j++)
        {
            if (mask[j] != '?' && pattern[j] != *(begin + i + j))
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return const_cast<char*>(begin + i);
        }
    }
    return nullptr;
}
[[nodiscard]] uintptr_t Engine::Tools::FindPattern(HANDLE proc, std::uintptr_t begin, std::uintptr_t end, const scan_data_t& data) {
    auto it = reinterpret_cast<unsigned char*>(begin);
    uintptr_t num_read = {};
    char buffer[4096] = {};
    DWORD oldprotect = {};

    while (it < reinterpret_cast<unsigned char*>(end)) {

        if (!VirtualProtectEx(proc, it, sizeof(buffer), PROCESS_VM_READ, &oldprotect)) 
            throw std::exception("VirtualProtectEx failed");
 
        if(!ReadProcessMemory(proc, it, &buffer, sizeof(buffer), &num_read))
            throw std::exception("ReadProcessMemory failed");

        if(!VirtualProtectEx(proc, it, sizeof(buffer), oldprotect, &oldprotect))
            throw std::exception("VirtualProtectEx failed");

        if (!num_read) 
            throw std::exception("nothing was read");
        
        const auto result = reinterpret_cast<uintptr_t>(ScanSection(data, reinterpret_cast<const char*>(it), num_read));

        if (result) {

            const auto offs = (reinterpret_cast<BYTE*>(result) - it);
            return reinterpret_cast<uintptr_t>(it + offs);

        }

        it += num_read;

    }

    return uintptr_t(0);
}

uintptr_t Engine::Tools::FindPatternFromProcess(const std::string& proc, const std::optional<std::string>& dll, const scan_data_t& data)
{
    auto process = Engine::process_t(proc);
    const auto handle = process.Open();

    if (handle == 0)
        throw std::exception("FindPatternFromProcess(): invalid handle");

    uintptr_t addr{};

    if (dll) {
        auto engine = Engine::module_t(process, *dll);
        addr = FindPattern(handle, engine.begin, engine.end, data);
    } else {
        addr = FindPattern(handle, process.begin, process.end, data);
    }

    process.Close();
    return addr;
}

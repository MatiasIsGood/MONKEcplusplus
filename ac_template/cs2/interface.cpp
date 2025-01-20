#include "interface.hpp"

#include <Windows.h>
#include <iostream>
#include <format>

using InstantiateInterfaceFn_t = void* (*)();
struct CInterfaceRegister
{
	InstantiateInterfaceFn_t fnCreate;
	const char* szName;
	CInterfaceRegister* pNext;
};

[[nodiscard]] static std::uint8_t* GetRelativeAddress(std::uint8_t* nAddressBytes, std::uint32_t nRVAOffset, std::uint32_t nRIPOffset)
{
	std::uint32_t nRVA = *reinterpret_cast<std::uint32_t*>(nAddressBytes + nRVAOffset);
	std::uint64_t nRIP = reinterpret_cast<std::uint64_t>(nAddressBytes) + nRIPOffset;

	return reinterpret_cast<std::uint8_t*>(nRVA + nRIP);
}

[[nodiscard]] void* GetInterfaceCtor(const char* library)
{
	const auto handle = GetModuleHandleA(library);

	if (!handle)
		return nullptr;

	const auto fnc = GetProcAddress(handle, "CreateInterface");

	return fnc;
}
template <typename T>
[[nodiscard]] T* GetInterface(const char* name, const char* library)
{
	auto fnc = GetInterfaceCtor(library);

	if (!fnc)
		return nullptr;

	return reinterpret_cast<T * (*)(const char*, std::size_t*)>(fnc)(name, nullptr);
}



[[nodiscard]] CInterfaceRegister* GetInterfaceList(const char* library)
{
	void* ctor = GetInterfaceCtor(library);

	if (!ctor)
		return nullptr;

	return *reinterpret_cast<CInterfaceRegister**>(GetRelativeAddress((std::uint8_t*)ctor, 3, 7));
}

std::vector<CStaticInterfaces::KeyValue> CStaticInterfaces::m_sInterfaces = {
	
};
std::unordered_map<std::string, void*> CStaticInterfaces::m_oInterfaces;

void CStaticInterfaces::Init()
{
	m_oInterfaces.clear();

	for (const auto& [k, v] : m_sInterfaces) {
		auto result = GetInterface<void>(k.c_str(), v.c_str());

		if (result) {
			m_oInterfaces[k] = result;
			//std::cout << std::format("\"{}\" from \"{}\" = {:x}\n", k, v, (std::size_t)result);
		} else {
			std::cout << std::format("CStaticInterfaces::Init() ignoring \"{}\" from \"{}\"\n", k, v) << '\n';
		}
	}
}

std::vector<std::pair<std::string, std::string>> CStaticInterfaces::m_sNestedInterfaces = {
	{"tier0.dll", "VEngineCvar007"}
};

void CStaticInterfaces::InitNested()
{

	for (const auto& [k, v] : m_sNestedInterfaces) {
		auto list = GetInterfaceList(k.c_str());

		while (list) {
			if (list->szName == v) {
				m_oInterfaces[v] = list->fnCreate();
				//std::cout << std::format("{}[{}] -> {:x}\n", k, v, reinterpret_cast<std::size_t>(m_oInterfaces[v]));
				break;
			}

			list = list->pNext;
		}

	}
}

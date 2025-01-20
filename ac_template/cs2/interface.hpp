#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class CStaticInterfaces
{
	using KeyValue = std::pair<std::string, std::string>;
public:

	static void Init();
	static void InitNested();

	template<typename T>
	[[nodiscard]] static inline T Get(const std::string& name) {
		if (!m_oInterfaces.contains(name))
			return nullptr;

		return reinterpret_cast<T>(m_oInterfaces.at(name));
	}

private:
	static std::vector<KeyValue> m_sInterfaces;
	static std::vector<std::pair<std::string, std::string>> m_sNestedInterfaces;
	static std::unordered_map<std::string, void*> m_oInterfaces;
};

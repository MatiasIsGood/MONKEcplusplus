#include "icvar.hpp"
#include "cs2/interface.hpp"

ConVarElem* ICvar::GetAllCvars()
{
	auto cvars = GetInterface();

	if (!cvars)
		return {};

	return cvars->m_oConVars.pMemory;

}
CConVar* ICvar::FindVar(const std::string& name) const noexcept {

	for (auto i : std::views::iota(0u, m_oConVars.nElementCount)) {
		CConVar* var = m_oConVars.pMemory[i].element;

		if (var && var->m_sName == name)
			return var;

	}

	return nullptr;
}


CConVar* ICvar::Find(const std::string& name)
{
	auto cvars = GetInterface();

	if (!cvars)
		return nullptr;

	return cvars->FindVar(name);
}
std::uint16_t ICvar::GetCount()
{
	auto cvars = GetInterface();

	if (!cvars)
		return 0u;

	return cvars->m_oConVars.nElementCount;
}
std::uint16_t ICvar::IndexOf(const std::string& name)
{
	auto icvar = GetInterface();

	if (!icvar)
		return 65535u;

	for (auto i : std::views::iota(0u, icvar->m_oConVars.nElementCount)) {
		CConVar* var = icvar->m_oConVars.pMemory[i].element;

		if (var && var->m_sName == name)
			return i;
	}

	return 65535u;
}
[[nodiscard]] ICvar* ICvar::GetInterface()
{
	static ICvar* cvars = nullptr;

	if (!cvars)
		cvars = CStaticInterfaces::Get<ICvar*>("VEngineCvar007");

	return cvars;
}
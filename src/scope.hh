#ifndef IZNA_SCOPE_HH
#define IZNA_SCOPE_HH

#include "value.hh"

#include <memory>
#include <unordered_map>

namespace izna {

struct scope {
	std::shared_ptr<scope> m_prev;
	std::unordered_map<std::string, value> m_var_table;

	explicit scope(std::shared_ptr<scope> prev):
		m_prev(prev),
		m_var_table()
	{}

	value getValue(const std::string &name)
	{
		auto var = m_var_table.find(name);
		if (var != m_var_table.end())
		{
			return var->second;
		}

		if (!m_prev)
		{
			return value();
		} else
		{
			return m_prev->getValue(name);
		}
	}

	bool setValue(const std::string &name, const value &v, bool add_if_missing = true)
	{
		if (m_prev && m_prev->setValue(name, v, false))
		{
			return true;
		}

		auto var = m_var_table.find(name);
		if (var != m_var_table.end())
		{
			var->second = v;
			return true;
		} else if (add_if_missing)
		{
			m_var_table[name] = v;
			return true;
		} else
		{
			return false;
		}
	}
};

} //izna

#endif//IZNA_FRAME_HH


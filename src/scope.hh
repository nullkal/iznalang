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

	value *getValue(const std::string &name)
	{
		auto var = m_var_table.find(name);
		if (var != m_var_table.end())
		{
			return &var->second;
		}

		if (!m_prev)
		{
			return nullptr;
		} else
		{
			return m_prev->getValue(name);
		}
	}

	void setValue(const std::string &name, const value &v)
	{
		m_var_table[name] = v;
	}

	void setValue(const std::string &name, value &&v)
	{
		m_var_table[name] = std::move(v);
	}
};

} //izna

#endif//IZNA_FRAME_HH


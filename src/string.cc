#include "string.hh"
#include "boolean.hh"
#include "integer.hh"

#include "exception.hh"

#include <boost/lexical_cast.hpp>

namespace izna {

string::string(std::string value):
	m_value(value)
{}

std::string string::ToString() const
{
	return m_value;
}

value string::Add(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return string(m_value + rhs_value);
	} else
	{
		throw type_error();
	}
}

value string::Subtract(const value& rhs) const
{
	throw type_error();
}

value string::Multiply(const value& rhs) const
{
	if (typeid(rhs) == typeid(integer))
	{
		auto rhs_value = static_cast<integer&>(rhs).m_value;

		std::string buf;
		for (int i = 0; i < rhs_value; ++i)
		{
			buf += m_value;
		}
		return string(buf);
	} else
	{
		throw type_error();
	}
}

value string::Divide(const value& rhs) const
{
	throw type_error();
}

value string::Modulo(const value& rhs) const
{
	throw type_error();
}

value string::LogicalOr(const value& rhs) const
{
	throw type_error();
}

value string::LogicalAnd(const value& rhs) const
{
	throw type_error();
}

value string::Eq(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return boolean(m_value == rhs_value);
	} else
	{
		throw type_error();
	}
}

value string::Ne(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return boolean(m_value != rhs_value);
	} else
	{
		throw type_error();
	}
}

value string::Less(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return boolean(m_value < rhs_value);
	} else
	{
		throw type_error();
	}
}

value string::LessEq(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return boolean(m_value <= rhs_value);
	} else
	{
		throw type_error();
	}
}

value string::Greater(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return boolean(m_value > rhs_value);
	} else
	{
		throw type_error();
	}
}

value string::GreaterEq(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return boolean(m_value >= rhs_value);
	} else
	{
		throw type_error();
	}
}

value string::Neg() const
{
	throw type_error();
}

} //izna


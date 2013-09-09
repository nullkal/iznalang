#include "integer.hh"
#include "boolean.hh"

#include "exception.hh"

#include <boost/lexical_cast.hpp>

namespace izna {

integer::integer(int value):
	m_value(value)
{}

std::string integer::ToString() const
{
	return boost::lexical_cast<std::string>(m_value);
}

value integer::Add(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return integer(m_value + rhs_value);
	} else
	{
		throw type_error();
	}
}

value integer::Subtract(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return integer(m_value - rhs_value);
	} else
	{
		throw type_error();
	}
}

value integer::Multiply(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return integer(m_value * rhs_value);
	} else
	{
		throw type_error();
	}
}

value integer::Divide(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return integer(m_value / rhs_value);
	} else
	{
		throw type_error();
	}
}

value integer::Modulo(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return integer(m_value % rhs_value);
	} else
	{
		throw type_error();
	}
}

value integer::LogicalOr(const value& rhs) const
{
	throw type_error();
}

value integer::LogicalAnd(const value& rhs) const
{
	throw type_error();
}

value integer::Eq(const value& rhs) const
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

value integer::Ne(const value& rhs) const
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

value integer::Less(const value& rhs) const
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

value integer::LessEq(const value& rhs) const
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

value integer::Greater(const value& rhs) const
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

value integer::GreaterEq(const value& rhs) const
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

value integer::Neg() const
{
	return integer(-m_value);
}

} //izna


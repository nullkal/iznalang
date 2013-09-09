#include "real.hh"
#include "boolean.hh"

#include "exception.hh"

#include <boost/lexical_cast.hpp>

namespace izna {

real::real(double value):
	m_value(value)
{}

std::string real::ToString() const
{
	return boost::lexical_cast<std::string>(m_value);
}

value real::Add(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return real(m_value + rhs_value);
	} else
	{
		throw type_error();
	}
}

value real::Subtract(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return real(m_value - rhs_value);
	} else
	{
		throw type_error();
	}
}

value real::Multiply(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return real(m_value * rhs_value);
	} else
	{
		throw type_error();
	}
}

value real::Divide(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return real(m_value / rhs_value);
	} else
	{
		throw type_error();
	}
}

value real::Modulo(const value& rhs) const
{
	throw type_error();
}

value real::LogicalOr(const value& rhs) const
{
	throw type_error();
}

value real::LogicalAnd(const value& rhs) const
{
	throw type_error();
}

value real::Eq(const value& rhs) const
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

value real::Ne(const value& rhs) const
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

value real::Less(const value& rhs) const
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

value real::LessEq(const value& rhs) const
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

value real::Greater(const value& rhs) const
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

value real::GreaterEq(const value& rhs) const
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

value real::Neg() const
{
	return real(-m_value);
}

} //izna


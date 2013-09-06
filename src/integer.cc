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

std::shared_ptr<value> integer::Add(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<integer>(m_value + rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::Subtract(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<integer>(m_value - rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::Multiply(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<integer>(m_value * rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::Divide(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<integer>(m_value / rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::Modulo(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<integer>(m_value % rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::LogicalOr(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> integer::LogicalAnd(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> integer::Eq(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<boolean>(m_value == rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::Ne(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<boolean>(m_value != rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::Less(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<boolean>(m_value < rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::LessEq(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<boolean>(m_value <= rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::Greater(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<boolean>(m_value > rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::GreaterEq(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<boolean>(m_value >= rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> integer::Neg() const
{
	return std::make_shared<integer>(-m_value);
}

} //izna


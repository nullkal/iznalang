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

std::shared_ptr<value> real::Add(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<real>(m_value + rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> real::Subtract(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<real>(m_value - rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> real::Multiply(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<real>(m_value * rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> real::Divide(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<real>(m_value / rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> real::Modulo(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> real::LogicalOr(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> real::LogicalAnd(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> real::Eq(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> real::Ne(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> real::Less(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> real::LessEq(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> real::Greater(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> real::GreaterEq(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> real::Neg() const
{
	return std::make_shared<real>(-m_value);
}

} //izna


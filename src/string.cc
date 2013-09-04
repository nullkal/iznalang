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

std::shared_ptr<value> string::Add(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<string>(m_value + rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> string::Subtract(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> string::Multiply(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(integer))
	{
		auto rhs_value = static_cast<integer&>(*rhs).m_value;

		std::string buf;
		for (int i = 0; i < rhs_value; ++i)
		{
			buf += m_value;
		}
		return std::make_shared<string>(buf);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> string::Divide(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> string::Modulo(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> string::LogicalOr(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> string::LogicalAnd(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> string::Eq(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> string::Ne(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> string::Less(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> string::LessEq(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> string::Greater(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> string::GreaterEq(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> string::Neg() const
{
	throw type_error();
}

} //izna


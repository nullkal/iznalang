#include "integer.hh"
#include "boolean.hh"

#include "exception.hh"

namespace izna {

boolean::boolean(bool value):
	m_value(value)
{}

std::string boolean::ToString() const
{
	if (m_value)
	{
		return "true";
	} else {
		return "false";
	}
}

std::shared_ptr<value> boolean::Add(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> boolean::Subtract(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> boolean::Multiply(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> boolean::Divide(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> boolean::Modulo(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> boolean::LogicalOr(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<integer>(m_value || rhs_value);
	} else
	{
		throw type_error();
	}
}

std::shared_ptr<value> boolean::LogicalAnd(std::shared_ptr<value> rhs) const
{
	if (typeid(*rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(*rhs).m_value;
		return std::make_shared<integer>(m_value && rhs_value);
	} else
	{
		throw type_error();
	}
}


std::shared_ptr<value> boolean::Eq(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> boolean::Ne(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> boolean::Less(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> boolean::LessEq(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> boolean::Greater(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> boolean::GreaterEq(std::shared_ptr<value> rhs) const
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

std::shared_ptr<value> boolean::Neg() const
{
	throw type_error();
}

} //izna


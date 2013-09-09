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

value boolean::Add(const value& rhs) const
{
	throw type_error();
}

value boolean::Subtract(const value& rhs) const
{
	throw type_error();
}

value boolean::Multiply(const value& rhs) const
{
	throw type_error();
}

value boolean::Divide(const value& rhs) const
{
	throw type_error();
}

value boolean::Modulo(const value& rhs) const
{
	throw type_error();
}

value boolean::LogicalOr(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return integer(m_value || rhs_value);
	} else
	{
		throw type_error();
	}
}

value boolean::LogicalAnd(const value& rhs) const
{
	if (typeid(rhs) == typeid(*this))
	{
		auto rhs_value = static_cast<decltype(*this)>(rhs).m_value;
		return integer(m_value && rhs_value);
	} else
	{
		throw type_error();
	}
}


value boolean::Eq(const value& rhs) const
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

value boolean::Ne(const value& rhs) const
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

value boolean::Less(const value& rhs) const
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

value boolean::LessEq(const value& rhs) const
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

value boolean::Greater(const value& rhs) const
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

value boolean::GreaterEq(const value& rhs) const
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

value boolean::Neg() const
{
	throw type_error();
}

} //izna


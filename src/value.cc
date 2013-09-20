#include "value.hh"
#include "exception.hh"

#include <algorithm>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace izna {

value::value():
	m_type(value_type::NIL),
	m_val(0)
{}

value::value(int v):
	m_type(value_type::INTEGER),
	m_val(v)
{}

value::value(double v):
	m_type(value_type::REAL),
	m_val(reinterpret_cast<intptr_t>(new double(v)))
{}

value::value(bool v):
	m_type(value_type::BOOLEAN),
	m_val(v)
{}

value::value(const std::string &v):
	m_type(value_type::STRING),
	m_val(reinterpret_cast<intptr_t>(new std::string(v)))
{}

value::value(std::shared_ptr<node> params, std::shared_ptr<node> stmt):
	m_type(value_type::FUNC),
	m_val(reinterpret_cast<intptr_t>(new func(params, stmt)))
{}

value::value(const value &v):
	m_type(v.m_type),
	m_val(v.m_val)
{
	if (v.isReal())
	{
		auto val = reinterpret_cast<double *>(v.m_val);
		m_val = reinterpret_cast<intptr_t>(new double(*val));
	}

	if (v.isString())
	{
		auto val = reinterpret_cast<std::string *>(v.m_val);
		m_val = reinterpret_cast<intptr_t>(new std::string(*val));
	}

	if (v.isFunc())
	{
		auto val = reinterpret_cast<func *>(v.m_val);
		m_val = reinterpret_cast<intptr_t>(new func(*val));
	}
}

value& value::operator=(const value &rhs)
{
	value tmp(rhs);
	this->swap(tmp);

	return *this;
}

void value::swap(value &b)
{
	std::swap(this->m_type, b.m_type);
	std::swap(this->m_val,  b.m_val);
}

value::~value()
{
	if (isReal())
	{
		delete reinterpret_cast<double *>(m_val);
	}

	if (isString())
	{
		delete reinterpret_cast<std::string *>(m_val);
	}

	if (isFunc())
	{
		delete reinterpret_cast<func *>(m_val);
	}
}

bool value::isNil() const
{
	return m_type == value_type::NIL;
}

bool value::isInteger() const
{
	return m_type == value_type::INTEGER;
}

bool value::isReal() const
{
	return m_type == value_type::REAL;
}

bool value::isBoolean() const
{
	return m_type == value_type::BOOLEAN;
}

bool value::isString() const
{
	return m_type == value_type::STRING;
}

bool value::isFunc() const
{
	return m_type == value_type::FUNC;
}

bool value::isTrue() const
{
	return m_type == value_type::BOOLEAN && m_val;
}

bool value::isFalse() const
{
	return m_type == value_type::BOOLEAN && !m_val;
}

int value::toInteger() const
{
	if (isInteger())
	{
		return m_val;
	}

	if (isReal())
	{
		return static_cast<int>(*reinterpret_cast<double *>(m_val));
	}

	throw type_error();
}

double value::toReal() const
{
	if (isInteger())
	{
		return static_cast<double>(m_val);
	}

	if (isReal())
	{
		return *reinterpret_cast<double *>(m_val);
	}

	throw type_error();
}

bool value::toBoolean() const
{
	if (isNil())
	{
		return false;
	}

	if (isBoolean())
	{
		return m_val;
	}

	if (isInteger())
	{
		return m_val;
	}

	if (isReal())
	{
		return reinterpret_cast<double *>(m_val);
	}

	if (isString())
	{
		return !reinterpret_cast<std::string *>(m_val)->empty();
	}

	throw type_error();
}

std::string value::toString() const
{
	if (isNil())
	{
		return "nil";
	}

	if (isFalse())
	{
		return "false";
	}

	if (isTrue())
	{
		return "true";
	}

	if (isInteger())
	{
		return boost::lexical_cast<std::string>(m_val);
	}

	if (isReal())
	{
		return boost::lexical_cast<std::string>(*reinterpret_cast<double *>(m_val));
	}

	if (isString())
	{
		return *reinterpret_cast<std::string *>(m_val);
	}

	if (isFunc())
	{
		return "function";
	}

	throw type_error();
}

func value::toFunc() const
{
	if (isFunc())
	{
		return *reinterpret_cast<func *>(m_val);
	}

	throw type_error();
}


#define ADD_OPERATE(type, opr) \
	if (is##type())\
	{\
		return value(this->to##type() opr rhs.to##type());\
	}

value value::Add(const value &rhs) const
{
	ADD_OPERATE(Boolean, +)
	ADD_OPERATE(Integer, +)
	ADD_OPERATE(Real, +)
	ADD_OPERATE(String, +)

	throw type_error();
}

value value::Sub(const value &rhs) const
{
	ADD_OPERATE(Boolean, -)
	ADD_OPERATE(Integer, -)
	ADD_OPERATE(Real, -)

	throw type_error();
}

value value::Mul(const value &rhs) const
{
	ADD_OPERATE(Boolean, *)
	ADD_OPERATE(Integer, *)
	ADD_OPERATE(Real, *)

	if (isString())
	{
		const int n = rhs.toInteger();
		const std::string s = toString();

		std::stringstream ss;
		for (int i = 0; i < n; ++i)
		{
			ss << s;
		}

		return value(ss.str());
	}

	throw type_error();
}

value value::Div(const value &rhs) const
{
	ADD_OPERATE(Boolean, /)
	ADD_OPERATE(Integer, /)
	ADD_OPERATE(Real, /)

	throw type_error();
}

value value::Mod(const value &rhs) const
{
	ADD_OPERATE(Boolean, %)
	ADD_OPERATE(Integer, %)

	throw type_error();
}

value value::LOr(const value &rhs) const
{
	return value(toBoolean() || rhs.toBoolean());
}

value value::LAnd(const value &rhs) const
{
	return value(toBoolean() && rhs.toBoolean());
}

value value::Eq(const value &rhs) const
{
	ADD_OPERATE(Boolean, ==)
	ADD_OPERATE(Integer, ==)
	ADD_OPERATE(Real, ==)
	ADD_OPERATE(String, ==)

	throw type_error();
}

value value::Ne(const value &rhs) const
{
	ADD_OPERATE(Boolean, !=)
	ADD_OPERATE(Integer, !=)
	ADD_OPERATE(Real, !=)
	ADD_OPERATE(String, !=)

	throw type_error();
}

value value::Less(const value &rhs) const
{
	ADD_OPERATE(Boolean, <)
	ADD_OPERATE(Integer, <)
	ADD_OPERATE(Real, <)
	ADD_OPERATE(String, <)

	throw type_error();
}

value value::LessEq(const value &rhs) const
{
	ADD_OPERATE(Boolean, <=)
	ADD_OPERATE(Integer, <=)
	ADD_OPERATE(Real, <=)
	ADD_OPERATE(String, <=)

	throw type_error();
}

value value::Greater(const value &rhs) const
{
	ADD_OPERATE(Boolean, >)
	ADD_OPERATE(Integer, >)
	ADD_OPERATE(Real, >)
	ADD_OPERATE(String, >)

	throw type_error();
}

value value::GreaterEq(const value &rhs) const
{
	ADD_OPERATE(Boolean, >=)
	ADD_OPERATE(Integer, >=)
	ADD_OPERATE(Real, >=)
	ADD_OPERATE(String, >=)

	throw type_error();
}

value value::Neg() const
{
	if (isInteger())
	{
		return value(-toInteger());
	}

	if (isReal())
	{
		return value(-toReal());
	}

	throw type_error();
}

} //izna


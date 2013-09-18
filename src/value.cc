#include "value.hh"
#include "exception.hh"

#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace izna {
namespace {

intptr_t MakeInteger(int v)
{
	auto new_obj = new integer;
	new_obj->h.type = value_type::INTEGER;
	new_obj->v = v;

	return reinterpret_cast<intptr_t>(new_obj);
}

intptr_t MakeReal(double v)
{
	auto new_obj = new real;
	new_obj->h.type = value_type::REAL;
	new_obj->v = v;

	return reinterpret_cast<intptr_t>(new_obj);
}

intptr_t MakeString(std::string v)
{
	auto new_obj = new string;
	new_obj->h.type = value_type::STRING;
	new_obj->v = v;

	return reinterpret_cast<intptr_t>(new_obj);
}

intptr_t MakeFunc(std::shared_ptr<node> params, std::shared_ptr<node> stmt)
{
	auto new_obj = new func;
	new_obj->h.type = value_type::FUNC;
	new_obj->params = params;
	new_obj->stmt   = stmt;

	return reinterpret_cast<intptr_t>(new_obj);
}

bool isConst(const value &v)
{
	return v.isFalse() || v.isTrue() || v.isNil();
}

} //izna::(anonymous)

value::value():
	m_val(NIL)
{}

value::value(int v):
	m_val(MakeInteger(v))
{}

value::value(double v):
	m_val(MakeReal(v))
{}

value::value(bool v):
	m_val(v ? TRUE : FALSE)
{}

value::value(const std::string &v):
	m_val(MakeString(v))
{}

value::value(std::shared_ptr<node> params, std::shared_ptr<node> stmt):
	m_val(MakeFunc(params, stmt))
{}

value::~value()
{}

bool value::isNil() const
{
	return m_val == NIL;
}

bool value::isInteger() const
{
	if (isConst(*this))
	{
		return false;
	}

	return reinterpret_cast<value_header *>(m_val)->type == value_type::INTEGER;
}

bool value::isReal() const
{
	if (isConst(*this))
	{
		return false;
	}

	return reinterpret_cast<value_header *>(m_val)->type == value_type::REAL;
}

bool value::isBoolean() const
{
	return m_val == FALSE || m_val == TRUE;
}

bool value::isString() const
{
	if (isConst(*this))
	{
		return false;
	}

	return reinterpret_cast<value_header *>(m_val)->type == value_type::STRING;
}

bool value::isFunc() const
{
	if (isConst(*this))
	{
		return false;
	}

	return reinterpret_cast<value_header *>(m_val)->type == value_type::FUNC;
}

bool value::isTrue() const
{
	return m_val == TRUE;
}

bool value::isFalse() const
{
	return m_val == FALSE;
}

int value::toInteger() const
{
	if (isInteger())
	{
		return reinterpret_cast<integer *>(m_val)->v;
	}

	if (isReal())
	{
		return static_cast<int>(reinterpret_cast<integer *>(m_val)->v);
	}

	throw type_error();
}

double value::toReal() const
{
	if (isInteger())
	{
		return static_cast<double>(reinterpret_cast<integer *>(m_val)->v);
	}

	if (isReal())
	{
		return reinterpret_cast<integer *>(m_val)->v;
	}

	throw type_error();
}

bool value::toBoolean() const
{
	if (isNil() || isFalse())
	{
		return false;
	}

	if (isTrue())
	{
		return true;
	}

	if (isInteger())
	{
		return reinterpret_cast<integer *>(m_val)->v != 0;
	}

	if (isReal())
	{
		return reinterpret_cast<real *>(m_val)->v != 0;
	}

	if (isString())
	{
		return !reinterpret_cast<string *>(m_val)->v.empty();
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
		return boost::lexical_cast<std::string>(reinterpret_cast<integer *>(m_val)->v);
	}

	if (isReal())
	{
		return boost::lexical_cast<std::string>(reinterpret_cast<real *>(m_val)->v);
	}

	if (isString())
	{
		return reinterpret_cast<string *>(m_val)->v;
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


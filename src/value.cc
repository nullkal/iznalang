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

value::value(native_func fun):
	m_type(value_type::NATIVE_FUNC),
	m_val(reinterpret_cast<intptr_t>(fun))
{}

value::value(const std::vector<value> &arr):
	m_type(value_type::ARRAY),
	m_val(reinterpret_cast<intptr_t>(new std::vector<value>(arr)))
{}

value::value(std::vector<value> &&arr):
	m_type(value_type::ARRAY),
	m_val(reinterpret_cast<intptr_t>(new std::vector<value>(arr)))
{}

value::value(const std::unordered_map<std::string, value> &arr):
	m_type(value_type::OBJECT),
	m_val(reinterpret_cast<intptr_t>(new std::unordered_map<std::string, value>(arr)))
{}

value::value(std::unordered_map<std::string, value> &&arr):
	m_type(value_type::OBJECT),
	m_val(reinterpret_cast<intptr_t>(new std::unordered_map<std::string, value>(arr)))
{}

value::value(value *ref):
	m_type(value_type::REF),
	m_val(reinterpret_cast<intptr_t>(ref))
{}

value::value(const value &v):
	m_type(value_type::NIL),
	m_val(0)
{
	m_val = v.m_val;
	if (!v.isRef())
	{
		if (v.isReal())
		{
			auto val = reinterpret_cast<double *>(v.m_val);
			m_val = reinterpret_cast<intptr_t>(new double(*val));
		} else if (v.isString())
		{
			auto val = reinterpret_cast<std::string *>(v.m_val);
			m_val = reinterpret_cast<intptr_t>(new std::string(*val));
		} else if (v.isFunc())
		{
			auto val = reinterpret_cast<func *>(v.m_val);
			m_val = reinterpret_cast<intptr_t>(new func(*val));
		} else if (v.isArray())
		{
			auto val = reinterpret_cast<std::vector<value> *>(v.m_val);
			m_val = reinterpret_cast<intptr_t>(new std::vector<value>(*val));
		} else if (v.isObject())
		{
			auto val = reinterpret_cast<std::unordered_map<std::string, value> *>(v.m_val);
			m_val = reinterpret_cast<intptr_t>(new std::unordered_map<std::string, value>(*val));
		}
	}

	m_type = v.m_type;
}

value& value::operator=(const value &rhs)
{
	value tmp(rhs);
	swap(tmp);

	return *this;
}

value::value(value &&v):
	m_type(value_type::NIL),
	m_val(0)
{
	swap(v);
}

value& value::operator=(value &&v)
{
	swap(v);
	return *this;
}

void value::swap(value &b) noexcept
{
	std::swap(this->m_type, b.m_type);
	std::swap(this->m_val,  b.m_val);
}

value::~value()
{
	if (!isRef())
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

		if (isArray())
		{
			delete reinterpret_cast<std::vector<value> *>(m_val);
		}

		if (isObject())
		{
			delete reinterpret_cast<std::unordered_map<std::string, value> *>(m_val);
		}
	}
}

bool value::isNil() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isNil();
	}
	else
	{
		return m_type == value_type::NIL;
	}
}

bool value::isInteger() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isInteger();
	}
	else
	{
		return m_type == value_type::INTEGER;
	}
}

bool value::isReal() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isReal();
	}
	else
	{
		return m_type == value_type::REAL;
	}
}

bool value::isBoolean() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isBoolean();
	}
	else
	{
		return m_type == value_type::BOOLEAN;
	}
}

bool value::isString() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isString();
	}
	else
	{
		return m_type == value_type::STRING;
	}
}

bool value::isFunc() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isFunc();
	}
	else
	{
		return m_type == value_type::FUNC;
	}
}

bool value::isNativeFunc() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isNativeFunc();
	}
	else
	{
		return m_type == value_type::NATIVE_FUNC;
	}
}



bool value::isTrue() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isTrue();
	}
	else
	{
		return m_type == value_type::BOOLEAN && m_val;
	}
}

bool value::isRef() const
{
	return m_type == value_type::REF;
}

bool value::isArray() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isArray();
	}
	else
	{
		return m_type == value_type::ARRAY;
	}
}

bool value::isObject() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isObject();
	}
	else
	{
		return m_type == value_type::OBJECT;
	}
}

bool value::isFalse() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->isFalse();
	}
	else
	{
		return m_type == value_type::BOOLEAN && !m_val;
	}
}

int value::toInteger() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->toInteger();
	}

	if (isNil())
	{
		return 0;
	}

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
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->toReal();
	}

	if (isNil())
	{
		return 0.0;
	}

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
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->toBoolean();
	}

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
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->toString();
	}

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

	if (isNativeFunc())
	{
		return "native function";
	}

	if (isArray())
	{
		auto &arr = toArray();
		
		auto str = std::string("[");
		for (auto it = arr.begin(); it != arr.end(); ++it)
		{
			str += it->toString();
			if (std::next(it) != arr.end())
			{
				str += ", ";
			}
		}
		str += "]";

		return str;
	}

	if (isObject())
	{
		auto &obj = toUnorderedMap();
		
		auto str = std::string("{");
		for (auto it = obj.begin(); it != obj.end(); ++it)
		{
			str += std::string("\"") + it->first + "\": " + it->second.toString();
			if (std::next(it) != obj.end())
			{
				str += ", ";
			}
		}
		str += "}";

		return str;
	}

	throw type_error();
}

func &value::toFunc() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->toFunc();
	}

	if (isFunc())
	{
		return *reinterpret_cast<func *>(m_val);
	}

	throw type_error();
}

native_func value::toNativeFunc() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->toNativeFunc();
	}

	if (isNativeFunc())
	{
		return *reinterpret_cast<native_func>(m_val);
	}

	throw type_error();
}

std::vector<value> &value::toArray() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->toArray();
	}

	if (isArray())
	{
		return *reinterpret_cast<std::vector<value> *>(m_val);
	}

	throw type_error();
}

std::unordered_map<std::string, value> &value::toUnorderedMap() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->toUnorderedMap();
	}

	if (isObject())
	{
		return *reinterpret_cast<std::unordered_map<std::string, value> *>(m_val);
	}

	throw type_error();
}

value &value::toRef() const
{
	return *reinterpret_cast<value *>(m_val);
}

#define ADD_OPERATE(type, opr) \
	if (is##type())\
	{\
		return value(this->to##type() opr rhs.to##type());\
	}

#define DEFAULT_PRIORITY(opr) \
	if (isReal() || rhs.isReal())\
	{\
		return value(this->toReal() opr rhs.toReal());\
	} else if (isInteger() || rhs.isInteger())\
	{\
		return value(this->toInteger() opr rhs.toInteger());\
	} else if (isBoolean() || rhs.isBoolean())\
	{\
		return value(this->toBoolean() opr rhs.toBoolean());\
	}

value value::Add(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->Add(rhs);
	}

	if (isArray())
	{
		std::vector<value> new_v(toArray());
		if (rhs.isArray())
		{
			auto &rhs_arr = rhs.toArray();
			new_v.insert(new_v.end(), rhs_arr.begin(), rhs_arr.end());
		}
		else
		{
			new_v.push_back(rhs);
		}

		return value(std::move(new_v));
	}

	if (isObject())
	{
		std::unordered_map<std::string, value> new_m(toUnorderedMap());
		if (rhs.isObject())
		{
			auto &rhs_map = rhs.toUnorderedMap();
			new_m.insert(rhs_map.begin(), rhs_map.end());
		}
		else
		{
			throw type_error();
		}
		return value(std::move(new_m));
	}

	if (isString() || rhs.isString())
	{
		return value(this->toString() + rhs.toString());
	}

	DEFAULT_PRIORITY(+)

	throw type_error();
}

value value::Sub(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->Sub(rhs);
	}

	DEFAULT_PRIORITY(-)

	throw type_error();
}

value value::Mul(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->Mul(rhs);
	}

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

	if (isArray())
	{
		const int n = rhs.toInteger();
		auto arr = toArray();

		std::vector<value> new_v;
		for (int i = 0; i < n; ++i)
		{
			new_v.insert(new_v.end(), arr.begin(), arr.end());
		}

		return value(new_v);
	}

	DEFAULT_PRIORITY(*)

	throw type_error();
}

value value::Div(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->Div(rhs);
	}

	DEFAULT_PRIORITY(/)

	throw type_error();
}

value value::Mod(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->Mod(rhs);
	}

	if (isInteger())
	{
		return value(toInteger() % rhs.toInteger());
	} else if (isBoolean())
	{
		return value(toBoolean() % rhs.toBoolean());
	}

	throw type_error();
}

value value::LOr(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->LOr(rhs);
	}

	return value(toBoolean() || rhs.toBoolean());
}

value value::LAnd(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->LAnd(rhs);
	}

	return value(toBoolean() && rhs.toBoolean());
}

value value::Eq(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->Eq(rhs);
	}

	ADD_OPERATE(Boolean, ==)
	ADD_OPERATE(Integer, ==)
	ADD_OPERATE(Real, ==)
	ADD_OPERATE(String, ==)

	throw type_error();
}

value value::Ne(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->Ne(rhs);
	}

	ADD_OPERATE(Boolean, !=)
	ADD_OPERATE(Integer, !=)
	ADD_OPERATE(Real, !=)
	ADD_OPERATE(String, !=)

	throw type_error();
}

value value::Less(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->Less(rhs);
	}

	ADD_OPERATE(Boolean, <)
	ADD_OPERATE(Integer, <)
	ADD_OPERATE(Real, <)
	ADD_OPERATE(String, <)

	throw type_error();
}

value value::LessEq(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->LessEq(rhs);
	}

	ADD_OPERATE(Boolean, <=)
	ADD_OPERATE(Integer, <=)
	ADD_OPERATE(Real, <=)
	ADD_OPERATE(String, <=)

	throw type_error();
}

value value::Greater(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->Greater(rhs);
	}

	ADD_OPERATE(Boolean, >)
	ADD_OPERATE(Integer, >)
	ADD_OPERATE(Real, >)
	ADD_OPERATE(String, >)

	throw type_error();
}

value value::GreaterEq(const value &rhs) const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->GreaterEq(rhs);
	}

	ADD_OPERATE(Boolean, >=)
	ADD_OPERATE(Integer, >=)
	ADD_OPERATE(Real, >=)
	ADD_OPERATE(String, >=)

	throw type_error();
}

value value::Neg() const
{
	if (isRef())
	{
		return reinterpret_cast<value *>(m_val)->Neg();
	}

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

value value::Assign(const value &rhs) const
{
	if (isRef())
	{
		if (rhs.isRef())
		{
			*reinterpret_cast<value *>(m_val) = rhs.toRef();
		}
		else
		{
			*reinterpret_cast<value *>(m_val) = rhs;
		}
		return *this;
	}
	
	throw type_error();
}

} //izna


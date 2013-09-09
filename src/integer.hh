#ifndef IZNA_INTEGER_HH
#define IZNA_INTEGER_HH

#include "value.hh"

namespace izna {

class integer: public value
{
public:
	explicit integer(int value = 0);

	std::string ToString() const;

	value Add(const value& rhs) const;
	value Subtract(const value& rhs) const;
	value Multiply(const value& rhs) const;
	value Divide(const value& rhs) const;
	value Modulo(const value& rhs) const;

	value LogicalOr(const value& rhs) const;
	value LogicalAnd(const value& rhs) const;

	value Eq(const value& rhs) const;
	value Ne(const value& rhs) const;
	value Less(const value& rhs) const;
	value LessEq(const value& rhs) const;
	value Greater(const value& rhs) const;
	value GreaterEq(const value& rhs) const;

	value Neg() const;

	int m_value;
};

} //izna

#endif

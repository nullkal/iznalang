#ifndef IZNA_INTEGER_HH
#define IZNA_INTEGER_HH

#include "value.hh"

namespace izna {

class integer: public value
{
public:
	explicit integer(int value = 0);

	std::string ToString() const;

	std::shared_ptr<value> Add(std::shared_ptr<value> rhs) const;
	std::shared_ptr<value> Subtract(std::shared_ptr<value>rhs) const;
	std::shared_ptr<value> Multiply(std::shared_ptr<value>rhs) const;
	std::shared_ptr<value> Divide(std::shared_ptr<value>rhs) const;
	std::shared_ptr<value> Modulo(std::shared_ptr<value>rhs) const;

	std::shared_ptr<value> LogicalOr(std::shared_ptr<value>rhs) const;
	std::shared_ptr<value> LogicalAnd(std::shared_ptr<value>rhs) const;

	std::shared_ptr<value> Eq(std::shared_ptr<value>rhs) const;
	std::shared_ptr<value> Ne(std::shared_ptr<value>rhs) const;
	std::shared_ptr<value> Less(std::shared_ptr<value>rhs) const;
	std::shared_ptr<value> LessEq(std::shared_ptr<value>rhs) const;
	std::shared_ptr<value> Greater(std::shared_ptr<value>rhs) const;
	std::shared_ptr<value> GreaterEq(std::shared_ptr<value>rhs) const;

	std::shared_ptr<value> Neg() const;

	int m_value;
};

} //izna

#endif

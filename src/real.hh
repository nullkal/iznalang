#ifndef IZNA_REAL_HH
#define IZNA_REAL_HH

#include "value.hh"

namespace izna {

class real: public value
{
public:
	explicit real(double value = 0);

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

	double m_value;
};

} //izna

#endif

#ifndef IZNA_FUNC_HH
#define IZNA_FUNC_HH

#include "value.hh"
#include "node.hh"

namespace izna {

class func: public value
{
public:
	explicit func(std::shared_ptr<node> params, std::shared_ptr<node> stmt);

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

	std::shared_ptr<node> m_params;
	std::shared_ptr<node> m_stmt;
};

} //izna

#endif

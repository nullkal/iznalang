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

	std::shared_ptr<node> m_params;
	std::shared_ptr<node> m_stmt;
};

} //izna

#endif

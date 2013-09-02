#ifndef IZNA_VALUE_HH
#define IZNA_VALUE_HH

#include <memory>
#include <string>

namespace izna {

class value
{
public:
	virtual ~value()
	{}

	virtual std::string ToString() const = 0;

	virtual std::shared_ptr<value> Add(std::shared_ptr<value> rhs) const = 0;
	virtual std::shared_ptr<value> Subtract(std::shared_ptr<value>rhs) const = 0;
	virtual std::shared_ptr<value> Multiply(std::shared_ptr<value>rhs) const = 0;
	virtual std::shared_ptr<value> Divide(std::shared_ptr<value>rhs) const = 0;
	virtual std::shared_ptr<value> Modulo(std::shared_ptr<value>rhs) const = 0;

	virtual std::shared_ptr<value> LogicalOr(std::shared_ptr<value>rhs) const = 0;
	virtual std::shared_ptr<value> LogicalAnd(std::shared_ptr<value>rhs) const = 0;

	virtual std::shared_ptr<value> Eq(std::shared_ptr<value>rhs) const = 0;
	virtual std::shared_ptr<value> Ne(std::shared_ptr<value>rhs) const = 0;
	virtual std::shared_ptr<value> Less(std::shared_ptr<value>rhs) const = 0;
	virtual std::shared_ptr<value> LessEq(std::shared_ptr<value>rhs) const = 0;
	virtual std::shared_ptr<value> Greater(std::shared_ptr<value>rhs) const = 0;
	virtual std::shared_ptr<value> GreaterEq(std::shared_ptr<value>rhs) const = 0;

	virtual std::shared_ptr<value> Neg() const = 0;
};

} //izna

#endif

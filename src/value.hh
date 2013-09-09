#ifndef IZNA_VALUE_HH
#define IZNA_VALUE_HH

#include <string>

namespace izna {

class value
{
public:
	virtual ~value()
	{}

	virtual std::unique_ptr<value> Clone() const = 0;
	virtual std::string ToString() const = 0;

	virtual void Add(const value& rhs) = 0;
	virtual void Subtract(const value& rhs) = 0;
	virtual void Multiply(const value& rhs) = 0;
	virtual void Divide(const value& rhs) = 0;
	virtual void Modulo(const value& rhs) = 0;

	virtual void LogicalOr(const value& rhs) = 0;
	virtual void LogicalAnd(const value& rhs) = 0;

	virtual void Eq(const value& rhs) = 0;
	virtual void Ne(const value& rhs) = 0;
	virtual void Less(const value& rhs) = 0;
	virtual void LessEq(const value& rhs) = 0;
	virtual void Greater(const value& rhs) = 0;
	virtual void GreaterEq(const value& rhs) = 0;

	virtual void Neg() = 0;
};

} //izna

#endif

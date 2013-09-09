#include "func.hh"

#include "exception.hh"

namespace izna {

func::func(std::shared_ptr<node> params, std::shared_ptr<node> stmt):
	m_params(params),
	m_stmt(stmt)
{}

std::string func::ToString() const
{
	return "<Func>";
}

value func::Add(const value& rhs) const
{
	throw type_error();
}

value func::Subtract(const value& rhs) const
{
	throw type_error();
}

value func::Multiply(const value& rhs) const
{
	throw type_error();
}

value func::Divide(const value& rhs) const
{
	throw type_error();
}

value func::Modulo(const value& rhs) const
{
	throw type_error();
}

value func::LogicalOr(const value& rhs) const
{
	throw type_error();
}

value func::LogicalAnd(const value& rhs) const
{
	throw type_error();
}

value func::Eq(const value& rhs) const
{
	throw type_error();
}

value func::Ne(const value& rhs) const
{
	throw type_error();
}

value func::Less(const value& rhs) const
{
	throw type_error();
}

value func::LessEq(const value& rhs) const
{
	throw type_error();
}

value func::Greater(const value& rhs) const
{
	throw type_error();
}

value func::GreaterEq(const value& rhs) const
{
	throw type_error();
}

value func::Neg() const
{
	throw type_error();
}

} //izna


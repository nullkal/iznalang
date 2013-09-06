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

std::shared_ptr<value> func::Add(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::Subtract(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::Multiply(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::Divide(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::Modulo(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::LogicalOr(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::LogicalAnd(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::Eq(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::Ne(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::Less(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::LessEq(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::Greater(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::GreaterEq(std::shared_ptr<value> rhs) const
{
	throw type_error();
}

std::shared_ptr<value> func::Neg() const
{
	throw type_error();
}

} //izna


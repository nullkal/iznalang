#ifndef IZNA_VALUE_HH
#define IZNA_VALUE_HH

#include <memory>
#include <string>
#include <cstdint>

namespace izna {

const intptr_t FALSE = 0;
const intptr_t TRUE  = 2;
const intptr_t NIL   = 4;

class node;
struct func;

class value
{
public:
	explicit value();
	explicit value(int v);
	explicit value(double v);
	explicit value(bool v);
	explicit value(const std::string &v);
	explicit value(std::shared_ptr<node> params, std::shared_ptr<node> stmt);

	virtual ~value();

	bool isNil() const;
	bool isInteger() const;
	bool isReal() const;
	bool isBoolean() const;
	bool isString() const;
	bool isFunc() const;

	bool isTrue() const;
	bool isFalse() const;

	int toInteger() const;
	double toReal() const;
	bool toBoolean() const;
	std::string toString() const;
	func toFunc() const;

	value Add(const value &rhs) const;
	value Sub(const value &rhs) const;
	value Mul(const value &rhs) const;
	value Div(const value &rhs) const;
	value Mod(const value &rhs) const;

	value LOr(const value &rhs) const;
	value LAnd(const value &rhs) const;

	value Eq(const value &rhs) const;
	value Ne(const value &rhs) const;
	value Less(const value &rhs) const;
	value LessEq(const value &rhs) const;
	value Greater(const value &rhs) const;
	value GreaterEq(const value &rhs) const;

	value Neg() const;

private:
	std::intptr_t m_val;
};

enum class value_type
{
	INTEGER,
	REAL,
	STRING,
	FUNC
};

struct value_header
{
	value_type type;
};

struct integer
{
	value_header h;
	int v;
};

struct real
{
	value_header h;
	double v;
};

struct string
{
	value_header h;
	std::string v;
};

struct func
{
	value_header h;
	std::shared_ptr<node> params;
	std::shared_ptr<node> stmt;
};

} //izna

#endif

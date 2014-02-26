#ifndef IZNA_VALUE_HH
#define IZNA_VALUE_HH

#include <memory>
#include <vector>
#include <string>
#include <cstdint>

namespace izna {

class node;
struct func;

enum class value_type
{
	NIL,

	BOOLEAN,
	INTEGER,
	REAL,
	STRING,
	FUNC,
	NATIVE_FUNC
};

class value;
typedef value (*native_func)(std::vector<value>);

class value
{
public:
	explicit value();
	explicit value(int v);
	explicit value(double v);
	explicit value(bool v);
	explicit value(const std::string &v);
	explicit value(std::shared_ptr<node> params, std::shared_ptr<node> stmt);
	explicit value(native_func func);

	value(const value &v);
	value& operator=(const value &rhs);

	value(value &&v);
	value& operator=(value &&v);

	void swap(value &b) noexcept;

	virtual ~value();

	void DeleteValue();

	bool isNil() const;
	bool isInteger() const;
	bool isReal() const;
	bool isBoolean() const;
	bool isString() const;
	bool isFunc() const;
	bool isNativeFunc() const;

	bool isTrue() const;
	bool isFalse() const;

	int toInteger() const;
	double toReal() const;
	bool toBoolean() const;
	std::string toString() const;
	func toFunc() const;
	native_func toNativeFunc() const;

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
	value_type m_type;
	std::intptr_t m_val;
};

struct func
{
	std::shared_ptr<node> params;
	std::shared_ptr<node> stmt;

	explicit func():
		params(),
		stmt()
	{}

	explicit func(std::shared_ptr<node> params, std::shared_ptr<node> stmt):
		params(params),
		stmt(stmt)
	{}
};

} //izna

#endif

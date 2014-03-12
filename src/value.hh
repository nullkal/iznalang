#ifndef IZNA_VALUE_HH
#define IZNA_VALUE_HH

#include <memory>
#include <vector>
#include <unordered_map>
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
	NATIVE_FUNC,
	ARRAY,
	OBJECT,
	REF,
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
	explicit value(native_func fun);
	explicit value(const std::vector<value> &arr);
	explicit value(std::vector<value> &&arr);
	explicit value(const std::unordered_map<std::string, value> &arr);
	explicit value(std::unordered_map<std::string, value> &&arr);
	explicit value(value *ref);

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
	bool isArray() const;
	bool isObject() const;
	bool isRef() const;

	bool isTrue() const;
	bool isFalse() const;

	int toInteger() const;
	double toReal() const;
	bool toBoolean() const;
	std::string toString() const;
	func &toFunc() const;
	native_func toNativeFunc() const;
	std::vector<value> &toArray() const;
	std::unordered_map<std::string, value> &toUnorderedMap() const;
	value &toRef() const;

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

	value Assign(const value &rhs) const;

private:
	value_type m_type;
	std::intptr_t m_val;
};

struct scope;
struct func
{
	std::shared_ptr<node> params;
	std::shared_ptr<node> stmt;

	std::shared_ptr<scope> closure_scope;

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

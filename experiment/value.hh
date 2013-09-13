#ifndef IZNA_VALUE_HH
#define IZNA_VALUE_HH

#include <string>

namespace izna {

class value
{
public:
	explicit value();
	explicit value(int v);
	explicit value(double v);
	explicit value(bool v);
	explicit value(const std::string v);

	virtual ~value()
	{}

	bool isFixnum() const;
	bool isReal() const;
	bool isBoolean() const;
	bool isString() const;

	int toFixnum() const;
	double toReal() const;
	bool toBoolean() const;
	std::string toString() const;

	static const intptr_t NIL   = 0;
	static const intptr_t FALSE = 1;
	static const intptr_t TRUE  = 2;

private:
	std::intptr_t m_val;
};

} //izna

#endif

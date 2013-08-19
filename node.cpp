#include "node.h"
#include "driver.h"

int node::eval(izna_driver *driver) const
{
	switch(m_op)
	{
		case OP_PLUS:
			return m_left->eval(driver) + m_right->eval(driver);
		case OP_MINUS:
			return m_left->eval(driver) - m_right->eval(driver);
		case OP_MULTIPLY:
			return m_left->eval(driver) * m_right->eval(driver);
		case OP_DIVIDE:
			return m_left->eval(driver) / m_right->eval(driver);
		case OP_MODULO:
			return m_left->eval(driver) % m_right->eval(driver);
		case OP_ASSIGN:
			driver->assign(m_string, m_right->eval(driver));
			return driver->value(m_string);
		case OP_NEG:
			return -m_left->eval(driver);
		case OP_VALUE:
			return driver->value(m_string);
		case OP_CONST:
			return m_value;
		default:
			return 0;
	}
}


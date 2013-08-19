#ifndef IZNA_NODE_H
#define IZNA_NODE_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>

enum {
	OP_PLUS,
	OP_MINUS,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_MODULO,
	OP_ASSIGN,
	OP_NEG,
	OP_VALUE,
	OP_CONST
};

class izna_driver;
class node {
public:
	node(int op, node *left, node *right = 0):
		m_op(op), m_left(left), m_right(right), m_value(0), m_string(0)
	{}

	node(int op, int value):
		m_op(op), m_left(0), m_right(0), m_value(value), m_string(0)
	{}

	node(int op, std::string *str, node *right = 0):
		m_op(op), m_left(0), m_right(right), m_value(0), m_string(str)
	{}

	~node()
	{
		delete m_left;
		delete m_right;
		delete m_string;
	}

	int eval(izna_driver *driver) const;

private:
	int m_op;
	int m_value;
	std::string *m_string;
	node *m_left;
	node *m_right;
};
#endif

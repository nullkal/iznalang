#ifndef IZNA_NODE_HH
#define IZNA_NODE_HH

#include <string>
#include <memory>

#include "value.hh"

namespace izna {

enum {
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_MODULO,
	OP_ASSIGN,
	OP_NEG,
	OP_LOGICAL_OR,
	OP_LOGICAL_AND,
	OP_EQ,
	OP_NE,
	OP_LESS,
	OP_LESS_EQ,
	OP_GREATER,
	OP_GREATER_EQ,
	OP_VALUE,
	OP_CONST,
	OP_CONTINUE,
	OP_IF,
	OP_WHILE,
	OP_NEXT,
	OP_BREAK,
	OP_PARAM,
	OP_ARG,
	OP_EXECFUNC
};

class node 
{
public:
	node(
		int op,
		std::shared_ptr<node> left  = nullptr,
		std::shared_ptr<node> right = nullptr,
		std::shared_ptr<node> cond  = nullptr):
			m_op(op), m_value(0), m_string(), m_left(left), m_right(right), m_cond(cond)
	{}

	node(int op, std::shared_ptr<value> v):
		m_op(op), m_value(v), m_string(), m_left(nullptr), m_right(nullptr), m_cond(nullptr)
	{}

	node(int op, std::string str, std::shared_ptr<node> right = nullptr):
		m_op(op), m_value(0), m_string(str), m_left(nullptr), m_right(right), m_cond(nullptr)
	{}

	int m_op;
	std::shared_ptr<value> m_value;
	std::string m_string;
	std::shared_ptr<node> m_left;
	std::shared_ptr<node> m_right;
	std::shared_ptr<node> m_cond;
};

} //izna

#endif

#ifndef IZNA_NODE_HH
#define IZNA_NODE_HH

#include <string>
#include <memory>

namespace izna {

enum {
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_MODULO,
	OP_ASSIGN,
	OP_NEG,
	OP_VALUE,
	OP_CONST,
	OP_CONTINUE
};

class node 
{
public:
	node(int op, std::shared_ptr<node> left, std::shared_ptr<node> right = nullptr):
		m_op(op), m_left(left), m_right(right), m_value(0), m_string()
	{}

	node(int op, int value):
		m_op(op), m_left(nullptr), m_right(nullptr), m_value(value), m_string()
	{}

	node(int op, std::string str, std::shared_ptr<node> right = nullptr):
		m_op(op), m_left(nullptr), m_right(right), m_value(0), m_string(str)
	{}

	int m_op;
	int m_value;
	std::string m_string;
	std::shared_ptr<node> m_left;
	std::shared_ptr<node> m_right;
};

} //izna

#endif

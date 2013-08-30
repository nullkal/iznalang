#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>

#include "parser.hh"

namespace izna {

std::unordered_map<std::string, int> var_table;

int eval_tree(std::shared_ptr<node> node)
{
	if (!node) {
		return 0;
	}

	switch (node->m_op)
	{
	case OP_ADD:
		return eval_tree(node->m_left) + eval_tree(node->m_right);

	case OP_SUBTRACT:
		return eval_tree(node->m_left) - eval_tree(node->m_right);

	case OP_MULTIPLY:
		return eval_tree(node->m_left) * eval_tree(node->m_right);

	case OP_DIVIDE:
		return eval_tree(node->m_left) / eval_tree(node->m_right);

	case OP_MODULO:
		return eval_tree(node->m_left) % eval_tree(node->m_right);

	case OP_LOGICAL_OR:
		return eval_tree(node->m_left) || eval_tree(node->m_right);

	case OP_LOGICAL_AND:
		return eval_tree(node->m_left) && eval_tree(node->m_right);

	case OP_EQ:
		return eval_tree(node->m_left) == eval_tree(node->m_right);

	case OP_NE:
		return eval_tree(node->m_left) != eval_tree(node->m_right);

	case OP_LESS:
		return eval_tree(node->m_left) < eval_tree(node->m_right);

	case OP_LESS_EQ:
		return eval_tree(node->m_left) <= eval_tree(node->m_right);

	case OP_GREATER:
		return eval_tree(node->m_left) > eval_tree(node->m_right);

	case OP_GREATER_EQ:
		return eval_tree(node->m_left) >= eval_tree(node->m_right);

	case OP_ASSIGN:
		return var_table[node->m_string] = eval_tree(node->m_right);

	case OP_NEG:
		return -eval_tree(node->m_left);

	case OP_VALUE:
		return var_table[node->m_string];

	case OP_CONST:
		return node->m_value;

	case OP_CONTINUE:
		if (node->m_left->m_op != OP_NEXT)
		{
			eval_tree(node->m_left);
			return eval_tree(node->m_right);
		}
		return 0;
	
	case OP_IF:
		if (eval_tree(node->m_cond))
		{
			return eval_tree(node->m_left);
		} else
		{
			return eval_tree(node->m_right);
		}

	case OP_WHILE:
		{
			int val = 0;
			while (eval_tree(node->m_cond))
			{
				val = eval_tree(node->m_left);
			}
			return val;
		}
	}
	return 0;
}

} //izna

int main(int argc, char *argv[])
{
	std::string str(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>());
	izna::parser_params params(str.c_str(), str.size());

	izna::parser parser(params);
	if (parser.parse() != 0 || !params.root)
	{
		std::cout << "Failed to parse the input file." << std::endl;
		return 1;
	}

	izna::eval_tree(params.root);
	for (auto &it: izna::var_table)
	{
		std::cout << it.first << ": " << it.second << std::endl;
	}

	return 0;
}


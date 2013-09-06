#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>
#include <stdexcept>

#include "parser.hh"
#include "value.hh"
#include "boolean.hh"
#include "func.hh"
#include "exception.hh"

namespace izna {

std::unordered_map<std::string, std::shared_ptr<value>> var_table;

class break_stmt: public std::exception {};
class next_stmt : public std::exception {};

std::shared_ptr<value> eval_tree(std::shared_ptr<node> node)
{
	if (!node) {
		return 0;
	}

	switch (node->m_op)
	{
	case OP_ADD:
		return eval_tree(node->m_left)->Add(eval_tree(node->m_right));

	case OP_SUBTRACT:
		return eval_tree(node->m_left)->Subtract(eval_tree(node->m_right));

	case OP_MULTIPLY:
		return eval_tree(node->m_left)->Multiply(eval_tree(node->m_right));

	case OP_DIVIDE:
		return eval_tree(node->m_left)->Divide(eval_tree(node->m_right));

	case OP_MODULO:
		return eval_tree(node->m_left)->Modulo(eval_tree(node->m_right));

	case OP_LOGICAL_OR:
		return eval_tree(node->m_left)->LogicalOr(eval_tree(node->m_right));

	case OP_LOGICAL_AND:
		return eval_tree(node->m_left)->LogicalAnd(eval_tree(node->m_right));

	case OP_EQ:
		return eval_tree(node->m_left)->Eq(eval_tree(node->m_right));

	case OP_NE:
		return eval_tree(node->m_left)->Ne(eval_tree(node->m_right));

	case OP_LESS:
		return eval_tree(node->m_left)->Less(eval_tree(node->m_right));

	case OP_LESS_EQ:
		return eval_tree(node->m_left)->LessEq(eval_tree(node->m_right));

	case OP_GREATER:
		return eval_tree(node->m_left)->Greater(eval_tree(node->m_right));

	case OP_GREATER_EQ:
		return eval_tree(node->m_left)->GreaterEq(eval_tree(node->m_right));

	case OP_ASSIGN:
		return var_table[node->m_string] = eval_tree(node->m_right);

	case OP_NEG:
		return eval_tree(node->m_left)->Neg();

	case OP_VALUE:
		return var_table[node->m_string];

	case OP_CONST:
		return node->m_value;

	case OP_CONTINUE:
		eval_tree(node->m_left);
		return eval_tree(node->m_right);

	case OP_IF:
		{
			auto result = eval_tree(node->m_cond);

			auto casted_result = std::dynamic_pointer_cast<boolean>(result);
			if (!casted_result)
			{
				throw type_error();
			}

			if (casted_result->m_value)
			{
				return eval_tree(node->m_left);
			} else
			{
				return eval_tree(node->m_right);
			}
		}

	case OP_NEXT:
		throw next_stmt();

	case OP_BREAK:
		throw break_stmt();

	case OP_WHILE:
		{
			std::shared_ptr<value> val = 0;
			for(;;)
			{
				auto result = eval_tree(node->m_cond);

				auto casted_result = std::dynamic_pointer_cast<boolean>(result);
				if (!casted_result)
				{
					throw type_error();
				}

				if (!casted_result->m_value)
				{
					break;
				}

				try {
					val = eval_tree(node->m_left);
				} catch (next_stmt)
				{
				} catch (break_stmt)
				{
					break;
				}
			}
			return val;
		}
	case OP_EXECFUNC:
		{
			auto raw_f = eval_tree(node->m_left);
			if (typeid(*raw_f) != typeid(func))
			{
				throw type_error();
			}

			auto f = static_cast<func&>(*raw_f);

			auto cur_param = f.m_params;
			auto cur_arg   = node->m_right;
			while (cur_param != nullptr)
			{
				var_table[cur_param->m_string] = eval_tree(cur_arg->m_left);
				cur_param = cur_param->m_right;
				cur_arg   = cur_arg->m_right;
			}

			return eval_tree(f.m_stmt);
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
		std::cout << it.first << ": " << it.second->ToString() << std::endl;
	}

	return 0;
}


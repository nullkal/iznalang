#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <memory>

#include "scope.hh"
#include "parser.hh"
#include "value.hh"
#include "exception.hh"

namespace izna {

std::shared_ptr<scope> cur_scope = nullptr;

class break_stmt: public std::exception {};
class next_stmt : public std::exception {};

void pushScope()
{
	cur_scope = std::make_shared<scope>(cur_scope);
}

void popScope()
{
	cur_scope = cur_scope->m_prev;
}

value eval_tree(std::shared_ptr<node> node)
{
	if (!node) {
		return value();
	}

	switch (node->m_op)
	{
	case OP_ADD:
		return eval_tree(node->m_left).Add(eval_tree(node->m_right));

	case OP_SUBTRACT:
		return eval_tree(node->m_left).Sub(eval_tree(node->m_right));

	case OP_MULTIPLY:
		return eval_tree(node->m_left).Mul(eval_tree(node->m_right));

	case OP_DIVIDE:
		return eval_tree(node->m_left).Div(eval_tree(node->m_right));

	case OP_MODULO:
		return eval_tree(node->m_left).Mod(eval_tree(node->m_right));

	case OP_LOGICAL_OR:
		return eval_tree(node->m_left).LOr(eval_tree(node->m_right));

	case OP_LOGICAL_AND:
		return eval_tree(node->m_left).LAnd(eval_tree(node->m_right));

	case OP_EQ:
		return eval_tree(node->m_left).Eq(eval_tree(node->m_right));

	case OP_NE:
		return eval_tree(node->m_left).Ne(eval_tree(node->m_right));

	case OP_LESS:
		return eval_tree(node->m_left).Less(eval_tree(node->m_right));

	case OP_LESS_EQ:
		return eval_tree(node->m_left).LessEq(eval_tree(node->m_right));

	case OP_GREATER:
		return eval_tree(node->m_left).Greater(eval_tree(node->m_right));

	case OP_GREATER_EQ:
		return eval_tree(node->m_left).GreaterEq(eval_tree(node->m_right));

	case OP_ASSIGN:
		{
			auto v = eval_tree(node->m_right);
			cur_scope->setValue(node->m_string, v);
			return v;
		}

	case OP_NEG:
		return eval_tree(node->m_left).Neg();

	case OP_VALUE:
		return cur_scope->getValue(node->m_string);

	case OP_CONST:
		return *(node->m_value);

	case OP_CONTINUE:
		eval_tree(node->m_left);
		return eval_tree(node->m_right);

	case OP_IF:
		{
			if (eval_tree(node->m_cond).toBoolean())
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
			value val = value();
			for(;;)
			{
				if (!eval_tree(node->m_cond).toBoolean())
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
			pushScope();

			auto f = eval_tree(node->m_left).toFunc();

			auto cur_param = f.params;
			auto cur_arg   = node->m_right;
			while (cur_param != nullptr)
			{
				cur_scope->setValue(cur_param->m_string, eval_tree(cur_arg->m_left));
				cur_param = cur_param->m_right;
				cur_arg   = cur_arg->m_right;
			}

			auto result = eval_tree(f.stmt);

			popScope();
			return result;
		}
	}
	return value();
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

	izna::pushScope();

	izna::eval_tree(params.root);
	for (auto &it: izna::cur_scope->m_var_table)
	{
		std::cout << it.first << ": " << it.second.toString() << std::endl;
	}

	return 0;
}


#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <random>
#include <cmath>
#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>

#include <GL/glew.h>
#include <GL/glut.h>

#include "scope.hh"
#include "parser.hh"
#include "value.hh"
#include "exception.hh"

#include "draw/input.h"
#include "draw/texture.h"
#include "draw/render_target.h"
#include "draw/draw2d.h"

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

value eval_tree(std::shared_ptr<node> node);

value ExecFunc(value func_val, std::vector<value> &&args)
{
	value result;
	if (func_val.isFunc())
	{
		auto &f = func_val.toFunc();

		auto previous_scope = cur_scope;
		cur_scope = f.closure_scope;
		pushScope();
		{
			auto cur_param = f.params;
			auto cur_arg   = args.begin();
			while (cur_param != nullptr)
			{
				bool pass_by_value = !cur_arg->isRef() || cur_arg->isArray() || cur_arg->isObject();
				cur_scope->setValue(
					cur_param->m_string,
					pass_by_value? *cur_arg : cur_arg->toRef());
				cur_param = cur_param->m_right;
				++cur_arg;
			}

			result = eval_tree(f.stmt);
		}
		popScope();
		cur_scope = previous_scope;
	} else if (func_val.isNativeFunc())
	{
		auto f = func_val.toNativeFunc();
		result = f(args);
	} else
	{
		throw type_error();
	}

	return result;
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
		return eval_tree(node->m_left).Assign(eval_tree(node->m_right));

	case OP_NEG:
		return eval_tree(node->m_left).Neg();

	case OP_IDENTIFIER:
		{
			auto v = cur_scope->getValue(node->m_string);
			if (!v)
			{
				throw unknown_identifier_error();
			}

			return value(v);
		}

	case OP_VAR:
		cur_scope->setValue(node->m_string, eval_tree(node->m_right));
		return value(cur_scope->getValue(node->m_string));

	case OP_CONST:
		return *(node->m_value);

	case OP_CLOSURE:
		{
			auto &func = node->m_value->toFunc();
			func.closure_scope = cur_scope;

			return *(node->m_value);
		}

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
			std::vector<value> args;
			auto cur_arg = node->m_right;
			while (cur_arg != nullptr)
			{
				args.push_back(eval_tree(cur_arg->m_left));
				cur_arg = cur_arg->m_right;
			}

			return ExecFunc(eval_tree(node->m_left), std::move(args));
		}
	case OP_ARRAY:
		{
			std::vector<value> arr;

			auto cur_elem = node->m_left;
			while (cur_elem != nullptr)
			{
				arr.push_back(eval_tree(cur_elem->m_left));
				cur_elem = cur_elem->m_right;
			}

			return value(std::move(arr));
		}
	case OP_OBJECT:
		{
			std::unordered_map<std::string, value> obj;

			auto cur_elem = node->m_left;
			while (cur_elem != nullptr)
			{
				obj[cur_elem->m_left->m_string] = eval_tree(cur_elem->m_left->m_right);
				cur_elem = cur_elem->m_right;
			}

			return value(std::move(obj));
		}
	case OP_INDEX:
		{
			auto lv = eval_tree(node->m_left);
			if (lv.isArray())
			{
				auto &arr = lv.toArray();

				int rv_int;
				if (node->m_right)
				{
					rv_int = eval_tree(node->m_right).toInteger();
				} else
				{
					rv_int = arr.size();
				}

				if (arr.size() <= rv_int)
				{
					while (arr.capacity() <= rv_int)
					{
						if (arr.capacity() == 0)
						{
							arr.reserve(1);
						} else
						{
							arr.reserve(arr.capacity() * 2);
						}
					}
					arr.resize(rv_int + 1);
				}

				return value(&arr[rv_int]);
			}
			else if (lv.isObject())
			{
				auto &obj = lv.toUnorderedMap();
				return value(&obj[eval_tree(node->m_right).toString()]);
			}
		}
	}
	return value();
}

} //izna

const int   WINDOW_WIDTH   = 800;
const int   WINDOW_HEIGHT  = 600;
const char *WINDOW_TITLE   = "iznagame";
const int   TIMER_INTERVAL = 16;

void Reshape(int w, int h);
void Timer(int);

bool InitResources();
void Draw();

std::vector<stg::Texture_ptr> g_textures;
izna::value g_drawfunc;

struct Bullet
{
	Bullet *next;

	int count;

	double x;
	double y;

	double speed;
	double direction;

	izna::value func;
};
boost::object_pool<Bullet> g_bulletPool;

Bullet *g_bulletHead;

stg::Texture_ptr player;
double player_x = 400;
double player_y = 500;

stg::input::Key GetKey(std::string str)
{
	if (str == "ESC")
	{
		return stg::input::Key::Esc();
	}
	else if(str == "UP")
	{
		return stg::input::Key::Up();
	}
	else if(str == "DOWN")
	{
		return stg::input::Key::Down();
	}
	else if(str == "LEFT")
	{
		return stg::input::Key::Left();
	}
	else if(str == "RIGHT")
	{
		return stg::input::Key::Right();
	}
	else
	{
		return stg::input::Key(str[0]);
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow(WINDOW_TITLE);
	glutDisplayFunc(Draw);
	glutReshapeFunc(Reshape);
	glutTimerFunc(TIMER_INTERVAL, Timer, 0);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "ERROR: Failed to initialize GLEW" << std::endl;
		return 1;
	}

	if (!GLEW_EXT_framebuffer_object)
	{
		std::cerr << "ERROR: EXT_framebuffer_objet is not supported in this environment" << std::endl;
		return 1;
	}

	if (!GLEW_ARB_vertex_buffer_object)
	{
		std::cerr << "ERROR: ARB_vertex_buffer_objet is not supported in this environment" << std::endl;
		return 1;
	}

	stg::input::Init();
	stg::InitDraw2D();
	if (!InitResources())
	{
		return 1;
	}

	{
		std::string str((std::istreambuf_iterator<char>(std::cin)), (std::istreambuf_iterator<char>()));
		izna::parser_params params(str.c_str(), str.size());

		izna::parser parser(params);
		if (parser.parse() != 0 || !params.root)
		{
			std::cout << "Failed to parse the input file." << std::endl;
			return 1;
		}

		izna::pushScope();

		izna::cur_scope->setValue(
			"sleep",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					usleep(args[0].toInteger());
					return izna::value();
				})
			);

		izna::cur_scope->setValue(
			"print",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					std::cout << args[0].toString();
					return izna::value();
				})
			);

		izna::cur_scope->setValue(
			"LoadPNG",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					g_textures.push_back(stg::LoadPNG(args[0].toString().c_str()));
					return izna::value(static_cast<int>(g_textures.size() - 1));
				})
			);

		izna::cur_scope->setValue(
			"SplitTexture",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					std::vector<stg::Texture_ptr> splitted_textures;
					if (args.size() >= 5)
					{
						splitted_textures = stg::SplitTexture(
							g_textures[args[0].toInteger()],
							args[1].toInteger(),
							args[2].toInteger(),
							args[3].toInteger(),
							args[4].toInteger());
					} else
					{
						splitted_textures = stg::SplitTexture(
							g_textures[args[0].toInteger()],
							args[1].toInteger(),
							args[2].toInteger());
					}

					int head_index = g_textures.size();
					g_textures.insert(
						g_textures.end(),
						splitted_textures.begin(),
						splitted_textures.end());

					std::vector<izna::value> retval;
					for (int i = 0; i < splitted_textures.size(); ++i)
					{
						retval.push_back(izna::value(i + head_index));
					}
					return izna::value(std::move(retval));
				})
			);

		izna::cur_scope->setValue(
			"MainLoop",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					g_drawfunc = args[0];
					return izna::value();
				})
			);

		izna::cur_scope->setValue(
			"Draw",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					stg::Drawer2D(
						g_textures[args[0].toInteger()],
						args[1].toInteger(),
						args[2].toInteger()).Apply();
					return izna::value();
				})
			);

		izna::cur_scope->setValue(
			"KeyIsPushed",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					return izna::value(GetKey(args[0].toString()).IsPushed());
				})
			);

		izna::cur_scope->setValue(
			"KeyIsHolded",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					return izna::value(GetKey(args[0].toString()).IsHolded());
				})
			);

		izna::cur_scope->setValue(
			"KeyIsReleased",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					return izna::value(GetKey(args[0].toString()).IsReleased());
				})
			);

		izna::cur_scope->setValue(
			"Draw",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					std::unordered_map<std::string, izna::value> options;
					if (args.size() >= 4 && args[3].isObject())
					{
						auto &opt_args = args[3].toUnorderedMap();
						options.insert(opt_args.begin(), opt_args.end());
					}

					stg::Drawer2D(
						g_textures[args[0].toInteger()],
						args[1].toInteger(),
						args[2].toInteger())
					.SetOrigin(options["origin_x"].toReal(), options["origin_y"].toReal())
					.Rotate(options["rotate"].toReal())
					.SetFlipX(options["flip_x"].toBoolean())
					.SetFlipY(options["flip_y"].toBoolean())
					.SetScale(
						options["scale_x"].toBoolean() ? options["scale_x"].toReal() : 1,
						options["scale_y"].toBoolean() ? options["scale_y"].toReal() : 1)
					.Apply();

					return izna::value();
				})
			);

		izna::cur_scope->setValue(
			"PutBullet",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					Bullet *new_bullet = g_bulletPool.construct();
					new_bullet->count = 0;
					new_bullet->x     = args[0].toReal();
					new_bullet->y     = args[1].toReal();
					new_bullet->speed = args[2].toReal();
					new_bullet->direction = args[3].toReal();
					new_bullet->func = args[4];

					new_bullet->next = g_bulletHead;
					g_bulletHead = new_bullet;

					return izna::value();
				})
			);

		izna::cur_scope->setValue(
			"DrawBullets",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					auto bullet_param = izna::value(std::unordered_map<std::string, izna::value>());
					auto &raw_bp = bullet_param.toUnorderedMap();

					auto prev_ip = &g_bulletHead;
					for (auto it = g_bulletHead; it != nullptr;)
					{
						raw_bp["count"]     = izna::value(it->count);
						raw_bp["x"]         = izna::value(it->x);
						raw_bp["y"]         = izna::value(it->y);
						raw_bp["speed"]     = izna::value(it->speed);
						raw_bp["direction"] = izna::value(it->direction);
						raw_bp["func"]      = izna::value(it->func);
						if (ExecFunc(it->func, {izna::value(&bullet_param)}).toBoolean())
						{
							++it->count;
							it->speed     = raw_bp["speed"].toReal();
							it->direction = raw_bp["direction"].toReal();
							it->x         = raw_bp["x"].toReal() + it->speed * cos(it->direction);
							it->y         = raw_bp["y"].toReal() + it->speed * sin(it->direction);
							it->func      = raw_bp["func"];

							prev_ip = &(it->next);
							it = it->next;
						} else
						{
							auto old_it = it;
							it = it->next;
							*prev_ip = it;
							g_bulletPool.destroy(old_it);
						}
					}
					return izna::value();
				})
			);

		izna::cur_scope->setValue(
			"sin",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					return izna::value(std::sin(args[0].toReal()));
				})
			);

		izna::cur_scope->setValue(
			"cos",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					return izna::value(std::cos(args[0].toReal()));
				})
			);

		izna::cur_scope->setValue(
			"tan",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					return izna::value(std::tan(args[0].toReal()));
				})
			);

		izna::cur_scope->setValue(
			"atan2",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					return izna::value(std::atan2(args[0].toReal(), args[1].toReal()));
				})
			);


		izna::cur_scope->setValue(
			"rand",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					static std::mt19937 mt;

					std::uniform_real_distribution<double> rand_range(0.0, 1.0);
					return izna::value(rand_range(mt));
				})
			);

		izna::cur_scope->setValue(
			"floor",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					return izna::value(floor(args[0].toReal()));
				})
			);

		izna::cur_scope->setValue(
			"ArraySize",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					return izna::value(static_cast<int>(args[0].toArray().size()));
				})
			);

		izna::cur_scope->setValue(
			"ArrayPushBack",
			izna::value([](std::vector<izna::value> args) -> izna::value {
					args[0].toArray().push_back(args[1]);
					return izna::value();
				})
			);

		izna::eval_tree(params.root);

		if (!g_drawfunc.isNil())
		{
			glutMainLoop();
		}
		izna::popScope();
	}

	return 0;
}


void Reshape(int w, int h)
{
	if (w != WINDOW_WIDTH || h != WINDOW_HEIGHT)
	{
		glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
}

void Timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(TIMER_INTERVAL, Timer, 0);
}

bool InitResources()
{
	player = stg::LoadPNG("player.png");

	return true;
}

void Draw()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		double player_dx = 0;
		double player_dy = 0;
		if (stg::input::Key::Up().IsHolded())
		{
			player_dy -= 1;
		}
		if (stg::input::Key::Down().IsHolded())
		{
			player_dy += 1;
		}
		if (stg::input::Key::Left().IsHolded())
		{
			player_dx -= 1;
		}
		if (stg::input::Key::Right().IsHolded())
		{
			player_dx += 1;
		}

		const double player_speed = stg::input::Key('z').IsHolded()? 4 : 8;
		const double norm = sqrt(player_dx * player_dx + player_dy * player_dy);
		if (norm != 0)
		{
			player_dx *= player_speed / norm;
			player_dy *= player_speed / norm;
		}

		player_x += player_dx;
		player_y += player_dy;
	}

	stg::Draw2D([](){
		if (player)
		{
		stg::Drawer2D(player, player_x, player_y)
				.SetOrigin(24, 24)
				.Apply();
		}

		izna::cur_scope->setValue("player_x", izna::value(player_x));
		izna::cur_scope->setValue("player_y", izna::value(player_y));

		ExecFunc(g_drawfunc, std::vector<izna::value>());
	});

	glFlush();
	glutSwapBuffers();

	stg::input::Update();
}

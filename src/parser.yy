
%skeleton "lalr1.cc"
%define parser_class_name {parser}
%define api.namespace     {izna}
%define api.value.type    variant
%defines
%locations

%code requires {
#include "node.hh"
#include "location.hh"

namespace izna {

enum class lex_state
{
	BEGIN,
	COMMENT
};

struct parser_params
{
	explicit parser_params(const char *input, size_t size):
		root(nullptr),
		state(lex_state::BEGIN),
		input(input),
		inputBegin(input),
		inputEnd(input + size)
	{}

	std::shared_ptr<node> root;
	lex_state state;

	const char *input;
	const char *inputBegin;
	const char *inputEnd;
};

} //izna
}

%parse-param { parser_params& params }
%lex-param   { parser_params& params }

%{
#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

#include "parser.hh"
#include "integer.hh"
#include "real.hh"
#include "string.hh"
#include "func.hh"

namespace izna {

parser::token_type yylex(
	parser::semantic_type* yylval,
	parser::location_type* yylloc,
	parser_params& params);

} //izna
%}

%token               TK_EOF     0 "end of file"

%token <std::string> IDENTIFIER   "identifier"
%token <int>         INTEGER      "integer"
%token <double>      REAL         "real"
%token <std::string> STRING       "string"

%token               EQ           "=="
%token               NE           "!="
%token               LESS         "<"
%token               LESS_EQ      "<="
%token               GREATER      ">"
%token               GREATER_EQ   ">="
%token               LOGICAL_OR   "||"
%token               LOGICAL_AND  "&&"

%token               END          "end"

%token               DO           "do"

%token               IF           "if statement"
%token               ELSIF        "elsif"
%token               ELSE         "else"

%token               WHILE        "while"
%token               NEXT         "next"
%token               BREAK        "break"


%type  <std::shared_ptr<node>>  compstmt
%type  <std::shared_ptr<node>>  stmt
%type  <std::shared_ptr<node>>  expr

%type  <std::string>            lvalue

%type  <std::shared_ptr<node>>  if_stmt
%type  <std::shared_ptr<node>>  opt_elsifs
%type  <std::shared_ptr<node>>  elsif
%type  <std::shared_ptr<node>>  opt_else
%type  <std::shared_ptr<node>>  else

%type  <std::shared_ptr<node>>  while_stmt
%type  <std::shared_ptr<node>>  next_stmt
%type  <std::shared_ptr<node>>  break_stmt

%type  <std::shared_ptr<node>>  do_stmt

%type  <std::shared_ptr<node>>  opt_args
%type  <std::shared_ptr<node>>  args
%type  <std::shared_ptr<node>>  opt_params
%type  <std::shared_ptr<node>>  params

%right '='
%left  LOGICAL_OR
%left  LOGICAL_AND
%left  EQ NE LESS LESS_EQ GREATER GREATER_EQ
%left  '+' '-'
%left  '*' '/' '%'
%left  NEG
%left  EXEC_FUNC

%%

program: compstmt opt_terms { params.root = $1; }
	   ;

compstmt:                     { $$ = nullptr; }
		| stmt                { $$ = $1; }
		| compstmt terms stmt { $$= std::make_shared<node>(OP_CONTINUE, $1, $3); }
		;

opt_terms: 
		 | terms
		 ;

terms: term
	 | terms term
	 ;

term: '\n'
	| ';'
	;

stmt: expr       { $$ = $1; }
	| if_stmt    { $$ = $1; }
	| while_stmt { $$ = $1; }
	| next_stmt  { $$ = $1; }
	| break_stmt { $$ = $1; }
	;

expr : expr '+' expr         { $$ = std::make_shared<node>(OP_ADD        , $1, $3); }
	 | expr '-' expr         { $$ = std::make_shared<node>(OP_SUBTRACT   , $1, $3); }
	 | expr '*' expr         { $$ = std::make_shared<node>(OP_MULTIPLY   , $1, $3); }
	 | expr '/' expr         { $$ = std::make_shared<node>(OP_DIVIDE     , $1, $3); }
	 | expr '%' expr         { $$ = std::make_shared<node>(OP_MODULO     , $1, $3); }
	 | expr LOGICAL_OR  expr { $$ = std::make_shared<node>(OP_LOGICAL_OR , $1, $3); }
	 | expr LOGICAL_AND expr { $$ = std::make_shared<node>(OP_LOGICAL_AND, $1, $3); }
	 | expr EQ expr          { $$ = std::make_shared<node>(OP_EQ         , $1, $3); }
	 | expr NE expr          { $$ = std::make_shared<node>(OP_NE         , $1, $3); }
	 | expr LESS    expr     { $$ = std::make_shared<node>(OP_LESS       , $1, $3); }
	 | expr LESS_EQ expr     { $$ = std::make_shared<node>(OP_LESS_EQ    , $1, $3); }
	 | expr GREATER    expr  { $$ = std::make_shared<node>(OP_GREATER    , $1, $3); }
	 | expr GREATER_EQ expr  { $$ = std::make_shared<node>(OP_GREATER_EQ , $1, $3); }
	 | lvalue '=' expr       { $$ = std::make_shared<node>(OP_ASSIGN     , $1, $3); }
	 | '-' expr %prec NEG    { $$ = std::make_shared<node>(OP_NEG        , $2); }
	 | expr '(' opt_args ')' %prec EXEC_FUNC { $$ = std::make_shared<node>(OP_EXECFUNC, $1, $3); }
	 | '(' expr ')'          { $$ = $2; }
	 | lvalue                { $$ = std::make_shared<node>(OP_VALUE, $1); }
	 | "integer"             { $$ = std::make_shared<node>(OP_CONST, std::make_shared<integer>($1)); }
	 | "real"                { $$ = std::make_shared<node>(OP_CONST, std::make_shared<real>($1)); }
	 | "string"              { $$ = std::make_shared<node>(OP_CONST, std::make_shared<string>($1)); }
	 | '\\' '(' opt_params ')' do_stmt
	     { $$ = std::make_shared<node>(OP_CONST, std::make_shared<func>($3, $5)); }
	 ;

lvalue: "identifier" { $$ = $1; }
	  ;

do_stmt: DO term compstmt term END { $$ = $3; }
	   | stmt { $$ = $1; }
	   ;

if_stmt: IF expr term compstmt term opt_elsifs opt_else END {
		     $$ = std::make_shared<node>(OP_IF, $4, $6, $2);
			 if ($7)
			 {
				 auto cur_node = $$;
				 while(cur_node->m_right)
				 {
					 cur_node = cur_node->m_right;
				 }
				 cur_node->m_right = $7;
			 }
		 }
		 ;

opt_elsifs: { $$ = nullptr; }
		  | opt_elsifs elsif {
				if ($1)
				{
					auto cur_node = $1;
					while(cur_node->m_right)
					{
						cur_node = cur_node->m_right;
					}
					cur_node->m_right = $2;
					$$ = $1;
				} else {
					$$ = $2;
				}
			}
		  ;

elsif: ELSIF expr term compstmt terms
	   { $$ = std::make_shared<node>(OP_IF, $4, nullptr, $2); }
	 ;

opt_else: { $$ = nullptr; }
		| else { $$ = $1; }
		;

else: ELSE term compstmt terms { $$ = $3; }
	;

while_stmt: WHILE expr term compstmt term END
		    { $$ = std::make_shared<node>(OP_WHILE, $4, nullptr, $2); }
		  ;

next_stmt: NEXT { $$ = std::make_shared<node>(OP_NEXT); }
		 ;

break_stmt: BREAK { $$ = std::make_shared<node>(OP_BREAK); }
		  ;

opt_params:        { $$ = nullptr; }
		  | params { $$ = $1; }
		  ;

params: lvalue             { $$ = std::make_shared<node>(OP_PARAM, $1); }
	  | lvalue ','  params { $$ = std::make_shared<node>(OP_PARAM, $1, $3); }
	  ;

opt_args:      { $$ = nullptr; }
		| args { $$ = $1; }
		;

args: expr          { $$ = std::make_shared<node>(OP_ARG, $1); }
	| expr ',' args { $$ = std::make_shared<node>(OP_ARG, $1, $3); }
	;

%%

namespace izna {

void parser::error(const location_type& loc, const std::string& msg)
{
	//TODO: Implement

	std::cout << msg << std::endl;
}

namespace {

class lex_checker
{
private:
	parser_params &m_params;

public:
	explicit lex_checker(parser_params &params):
		m_params(params)
	{}

	bool WhetherReachedEOF()
	{
		return m_params.input >= m_params.inputEnd;
	}

	boost::optional<char> Read(bool steps = true)
	{
		if (WhetherReachedEOF())
		{
			return boost::none;
		}

		return steps? *(m_params.input++) : *m_params.input;
	}

	boost::optional<char> ReadIfInputIs(char character, bool steps = true)
	{
		if (WhetherReachedEOF())
		{
			return boost::none;
		}

		if (*m_params.input != character)
		{
			return boost::none;
		}

		return steps? *(m_params.input++) : *m_params.input;
	}

	boost::optional<char> ReadIfInputIsIn(char rangeBegin, char rangeEnd, bool steps = true)
	{
		if (WhetherReachedEOF())
		{
			return boost::none;
		}

		if (*m_params.input < rangeBegin || rangeEnd < *m_params.input)
		{
			return boost::none;
		}

		return steps? *(m_params.input++) : *m_params.input;
	}

	boost::optional<char> ReadIfInputSatisfies(std::function<bool(char)> condition, bool steps = true)
	{
		if (WhetherReachedEOF())
		{
			return boost::none;
		}

		if (!condition(*m_params.input))
		{
			return boost::none;
		}

		return steps? *(m_params.input++) : *m_params.input;
	}

	bool DiscardIfInputIs(const char *str, size_t length = 0)
	{
		if (length == 0)
		{
			length = strlen(str);
		}

		for (int i = 0; i < length; ++i)
		{
			if (m_params.inputEnd <= m_params.input + i)
			{
				return false;
			}

			if (m_params.input[i] != str[i])
			{
				return false;
			}
		}

		m_params.input += length;
		return true;
	}
};

} //izna::(anonymous)

parser::token_type yylex(
	parser::semantic_type* yylval,
	parser::location_type* yylloc,
	parser_params& params)
{
	lex_checker chk(params);
	boost::optional<char> c;

	while (chk.ReadIfInputIs(' ') || chk.ReadIfInputIs('\t'));

	if (chk.ReadIfInputIs('#'))
	{
		while (!chk.ReadIfInputIs('\n', false))
		{
			++params.input;
		}
	}

	if (chk.ReadIfInputIs('0') && !chk.ReadIfInputIsIn('0', '9', false))
	{
		yylval->build<int>() = 0;
		return parser::token::INTEGER;
	}

	if (c = chk.ReadIfInputIsIn('1', '9'))
	{
		std::string buf;
		buf.append(1, *c);
		
		bool appeared_point = false;
		bool appeared_exp   = false;
		for (;;)
		{
			if (c = chk.ReadIfInputIsIn('0', '9'))
			{
				buf.append(1, *c);
			} else if (c = chk.ReadIfInputIs('.', false))
			{
				if (appeared_point || appeared_exp)
				{
					break;
				}
				appeared_point = true;

				buf.append(1, *c);
				++params.input;
			} else if ((c = chk.ReadIfInputIs('e', false)) || (c = chk.ReadIfInputIs('E', false)))
			{
				if (appeared_exp)
				{
					break;
				}
				appeared_exp = true;

				buf.append(1, *c);
				++params.input;

				if ((c = chk.ReadIfInputIs('+')) || (c = chk.ReadIfInputIs('-')))
				{
					buf.append(1, *c);
				} else
				{
					// TODO: implement notifying about the erroneous notation of floating number.
					break;
				}
			} else
			{
				break;
			}
		}

		if (appeared_point || appeared_exp)
		{
			yylval->build<double>() = boost::lexical_cast<double>(buf);
			return parser::token::REAL;
		} else
		{
			yylval->build<int>() = boost::lexical_cast<int>(buf);
			return parser::token::INTEGER;
		}
	}

	if (c = chk.ReadIfInputIs('"'))
	{
		std::string buf;
		for (;;)
		{
			if (c = chk.ReadIfInputIs('"'))
			{
				break;
			} else if (c = chk.ReadIfInputIs('\\'))
			{
				switch (*chk.Read())
				{
				case 'n':
					buf.append(1, '\n');
					break;
				case '"':
					buf.append(1, '"');
					break;
				default:
					// TODO: dispatch an error, or at least notify a warning
					break;
				}
			} else if (c = chk.ReadIfInputIs('\n'))
			{
				// TODO: dispatch an error
				break;
			} else
			{
				c = chk.Read();
				buf.append(1, *c);
			}
		}

		yylval->build<std::string>() = buf;
		return parser::token::STRING;
	}

	if (chk.DiscardIfInputIs("||"))
	{
		return parser::token::LOGICAL_OR;
	}

	if (chk.DiscardIfInputIs("&&"))
	{
		return parser::token::LOGICAL_AND;
	}

	if (chk.DiscardIfInputIs("=="))
	{
		return parser::token::EQ;
	}

	if (chk.DiscardIfInputIs("!="))
	{
		return parser::token::NE;
	}

	if (chk.DiscardIfInputIs("<="))
	{
		return parser::token::LESS_EQ;
	}

	if (chk.DiscardIfInputIs("<"))
	{
		return parser::token::LESS;
	}

	if (chk.DiscardIfInputIs(">="))
	{
		return parser::token::GREATER_EQ;
	}

	if (chk.DiscardIfInputIs(">"))
	{
		return parser::token::GREATER;
	}

	if (chk.DiscardIfInputIs("end"))
	{
	return parser::token::END;
	}

	if (chk.DiscardIfInputIs("do"))
	{
		return parser::token::DO;
	}

	if (chk.DiscardIfInputIs("if"))
	{
		return parser::token::IF;
	}

	if (chk.DiscardIfInputIs("elsif"))
	{
		return parser::token::ELSIF;
	}

	if (chk.DiscardIfInputIs("else"))
	{
		return parser::token::ELSE;
	}

	if (chk.DiscardIfInputIs("while"))
	{
		return parser::token::WHILE;
	}

	if (chk.DiscardIfInputIs("next"))
	{
		return parser::token::NEXT;
	}

	if (chk.DiscardIfInputIs("break"))
	{
		return parser::token::BREAK;
	}

	if ((c = chk.ReadIfInputSatisfies(isalpha)) || (c = chk.ReadIfInputIs('_')))
	{
		std::string str;
		do {
			str.append(1, *c);
		} while((c = chk.ReadIfInputSatisfies(isalnum)) || (c = chk.ReadIfInputIs('_')));

		yylval->build<std::string>() = str;
		return parser::token::IDENTIFIER;
	}

	if (c = chk.Read())
	{
		return parser::token_type(*c);
	}

	return parser::token::TK_EOF;
}

} //izna


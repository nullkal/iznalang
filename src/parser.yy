
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

struct parser_params
{
	explicit parser_params(const char *input, size_t size):
		root(nullptr), input(input), inputBegin(input), inputEnd(input + size)
	{}

	std::shared_ptr<node> root;

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
#include <boost/optional.hpp>

#include "parser.hh"

namespace izna {

parser::token_type yylex(
	parser::semantic_type* yylval,
	parser::location_type* yylloc,
	parser_params& params);

} //izna
%}

%token               END        0 "end of file"
%token <std::string> IDENTIFIER   "identifier"
%token <int>         NUMBER       "number"

%type  <std::shared_ptr<node>>  compstmt
%type  <std::shared_ptr<node>>  stmt
%type  <std::shared_ptr<node>>  expr

%right '='
%left  '+' '-'
%left  '*' '/' '%'
%left  NEG

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
	;

stmt: expr { $$ = $1; }
	;

expr : expr '+' expr         { $$ = std::make_shared<node>(OP_ADD     , $1, $3); }
	 | expr '-' expr         { $$ = std::make_shared<node>(OP_SUBTRACT, $1, $3); }
	 | expr '*' expr         { $$ = std::make_shared<node>(OP_MULTIPLY, $1, $3); }
	 | expr '/' expr         { $$ = std::make_shared<node>(OP_DIVIDE  , $1, $3); }
	 | expr '%' expr         { $$ = std::make_shared<node>(OP_MODULO  , $1, $3); }
	 | "identifier" '=' expr { $$ = std::make_shared<node>(OP_ASSIGN  , $1, $3); }
	 | '-' expr %prec NEG    { $$ = std::make_shared<node>(OP_NEG     , $2); }
	 | '(' expr ')'          { $$ = $2; }
	 | "identifier"          { $$ = std::make_shared<node>(OP_VALUE, $1); }
	 | "number"              { $$ = std::make_shared<node>(OP_CONST, $1); }
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

	if (chk.ReadIfInputIs('0') && !chk.ReadIfInputIsIn('0', '9', false))
	{
		yylval->build<int>() = 0;
		return parser::token::NUMBER;
	}

	if (c = chk.ReadIfInputIsIn('1', '9'))
	{
		int num = *c - '0';
		while (c = chk.ReadIfInputIsIn('0', '9'))
		{
			num = num * 10 + (*c - '0');
		}

		yylval->build<int>() = num;
		return parser::token::NUMBER;
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

	return parser::token::END;
}

} //izna


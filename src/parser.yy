
%skeleton "lalr1.cc"
%define parser_class_name {parser}
%define api.namespace     {izna}
%define api.value.type    variant
%defines
%locations

%{

#include <string>
#include <memory>
#include "node.hh"

%}

%code requires {

namespace izna {

struct parser_params
{
	std::shared_ptr<node> root;

	char *inputBegin;
	char *inputEnd;

	char *input;
};

} //izna

}

%parse-param { parser_params& params }
%lex-param   { parser_params& params }

%token               EOF        "end of file"
%token <std::string> IDENTIFIER "identifier"
%token <int>         NUMBER     "number"

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

compstmt:                     { $$ = std::make_shared<node>(nullptr); }
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
	 | '-' expr %prec NEG    { $$ = std:;make_shared<node>(OP_NEG     , $2); }
	 | '(' expr ')'          { $$ = $2; }
	 | "identifier"          { $$ = std::make_shared<node>(OP_VALUE, $1); }
	 | "number"              { $$ = std::make_shared<node>(OP_CONST, $1); }
	 ;

%%

namespace izna {

void parser::error(const location_type& loc, const std::string& msg)
{
	//TODO: Implement
}

parser::token_type yylex(parser::semantic_type* lval, parser_params& params)
{
	for (;;)
	{
		if (params.input == params.inputEnd)
		{
			return parser::token::EOF;
		}

		switch (*params.input)
		{
		case ' ':
		case '\t':
			++params.input;
			continue;

		case '0':
			++params.input;
			lval.build<int>() = 0;
			return parser::token::NUMBER;

		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			int num = 0;
			for(; params.input != params.inputEnd; ++params.input)
			{
				if (*params.input < '0' || '9' < *params.input)
				{
					break;
				}
				num = num * 10 + (*params.input - '0');
			}

			lval.build<int>() = num;
			return parser::token::NUMBER;

		default:
			return parser::token_type(*params.input++);
		}
	}
}

} //izna


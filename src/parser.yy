
%skeleton "lalr1.cc"
%define parser_class_name parser
%define api.namespace     izna
%define variant true
%defines

%{

#include <string>
#include <memory>
#include "node.hh"

namespace izna {

class izna_driver;

} //izna

%}

%parse-param { driver& drv }
%lex-param   { driver& drv }

%error-verbose

%{
#include "driver.hh"
%}

%token               TK_EOF        0 "end of file"
%token <int>         TK_INTEGER      "integer"
%token <std::string> TK_IDENTIFIER   "identifier"

%type <std::unique_ptr<node>> expr

%right '='
%left  '+' '-'
%left  '*' '/' '%'
%left  NEG

%%

program: compstmt opt_terms { $1->eval(); }
	   ;

compstmt:                     { $$ = std::unique_ptr<node>(nullptr); }
		| stmt                { $$ = $1; }
		| compstmt terms stmt { $$ = std::unique_ptr<node>(new node(OP_CONTINUE, $1, $3)); }
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

expr : expr '+' expr         { $$ = std::unique_ptr<node>(OP_ADD     , $1, $3); }
	 | expr '-' expr         { $$ = std::unique_ptr<node>(OP_SUBTRACT, $1, $3); }
	 | expr '*' expr         { $$ = std::unique_ptr<node>(OP_MULTIPLY, $1, $3); }
	 | expr '/' expr         { $$ = std::unique_ptr<node>(OP_DIVIDE  , $1, $3); }
	 | expr '%' expr         { $$ = std::unique_ptr<node>(OP_MODULO  , $1, $3); }
	 | "identifier" '=' expr { $$ = std::unique_ptr<node>(OP_ASSIGN  , $1, $3); }
	 | '-' expr %prec NEG    { $$ = std::unique_ptr<node>(OP_NEG     , $1, $3); }
	 | '(' expr ')'          { $$ = $2; }
	 | "identifier"          { $$ = std::unique_ptr<node>(OP_VALUE, $1); }
	 | "value"               { $$ = std::unique_ptr<node>(OP_CONST, $1); }
	 ;

%%

void izna::parser::error(
	const izna::parser::location_type&,
	const std::string &m)
{
	drv.error(m);
}


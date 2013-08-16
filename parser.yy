%skeleton "lalr1.cc"
%define "parser_class_name" "izna_parser"
%defines

%{
#include <string>

class izna_driver;
%}

%parse-param { izna_driver& driver }
%lex-param   { izna_driver& driver }

%error-verbose
%union
{
	int value;
	std::string *str;
	node *expr;
}

%token         TK_EOF   0 "end of file"
%token <value> TK_VALUE   "value"
%token <str>   TK_IDENT   "identifier"

%type <expr> expr

%destructor { delete $$; } "identifier"
%destructor { delete $$; } expr

%right '='
%left  '+' '-'
%left  '*' '/' '%'
%left  NEG;

%%
%start expr;

expr : expr '+' expr         { $$ = new node(OP_PLUS, $1, $3); }
	 | expr '-' expr         { $$ = new node(OP_MINUS, $1, $3); }
	 | expr '*' expr         { $$ = new node(OP_MULTIPLY, $1, $3); }
	 | expr '/' expr         { $$ = new node(OP_DIVIDE, $1, $3); }
	 | expr '%' expr         { $$ = new node(OP_MODULO, $1, $3); }
	 | "identifier" '=' expr { $$ = new node(OP_ASSIGN, $1, $3); }
	 | '-' expr %prec NEG    { $$ = new node(OP_NEG, $2); }
	 | '(' expr ')'          { $$ = $2 }
	 | "identifier"          { $$ = new node(OP_VALUE, $1); }
	 | "value"               { $$ = new node(OP_CONST, $1); }
	 ;
%%

void yy::izna_parser::error(
	const yy::izna_parser::location_type&, const std::string& m)
{
	driver.error(m);
}


%{
#include <cerrno>
#include <climits>
#include <string>
#include "driver.h"
#include "parser.hh"

#ifdef _MSC_VER
#pragma warning(disable:4018)
#pragma warning(disable:4102)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4996)
#endif

#undef yywrap
#define yywrap() 1

#define yyterminate()	return token::TK_EOF
%}

%option noyywrap nounput batch
%option noyy_scan_buffer
%option noyy_scan_bytes
%option noyy_scan_string
%option always-interactive

id    [a-zA-Z_][a-zA-Z_0-9]*
int   [1-9][0-9]*
blank [ \t]

%%
%{
	typedef yy::izna_parser::token token;

	std::string string_buffer;
%}

[-+*/%=()\n]		return yy::izna_parser::token_type(yytext[0]);

{blank}+		;
{int}			{
					errno = 0;
					long n = strtol(yytext, NULL, 10);
					if (n < LONG_MIN || n > LONG_MAX || errno == ERANGE)
						driver.error("整数が範囲外です。");
					yylval->value = n;
					return token::TK_VALUE;
				}
"0"				{
					yylval->value = 0;
					return token::TK_VALUE;
				}
{id}			{
					yylval->str = new std::string(yytext);
					return token::TK_IDENT;
				}
.				driver.error("この文字を識別子で使用することはできません。");

%%

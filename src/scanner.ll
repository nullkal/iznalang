%{
#include <cerrno>
#include <climits>
#include <string>
#include "driver.h"
#include "parser.hh"

#undef  yywrap
#define yywrap() 1

#define yyterminate() return token::TK_EOF;
%}

%option noyywrap nounput batch
%option noyy_scan_buffer
%option noyy_scan_bytes
%option noyy_scan_string
%option always-interactive

ident [a-zA-Z_][a-zA-Z_0-9]*
int   [1-9][0-9]*
blank [ \t]

%%
%{
	typedef izna::parser::token token;

	std::string string_buffer;
%}

[-+*/%=()\n]
{
	return izna::parser::token_type(yytext[0]);
}

{blank}+
{}

{int}
{
	errno = 0;
	long n = strtol(yytext, NULL, 10);
	if (n < LONG_MIN || n > LONG_MAX|| errno == ERANGE)
	{
		driver.error("整数が範囲外です。");
	}

	yylval.build<int>() = n;
	return token::TK_INTEGER;
}

"0"
{
	yylval.build<int>() = 0;
	return token::TK_INTEGER;
}

{id}
{
	yylval.build<std::string>() = std::string(yytext);
	return token::TK_IDENT;
}

.
{
	driver.error("この文字を識別子で使用することはできません。");
}

%%


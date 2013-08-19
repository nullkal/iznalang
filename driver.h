#ifndef IZNA_DRIVER_H
#define IZNA_DRIVER_H

#include "parser.hh"
#include "node.h"
#include <map>
#include <string>

class izna_driver;

#define YY_DECL										\
	yy::izna_parser::token_type						\
	yylex(yy::izna_parser::semantic_type* yylval,	\
		 izna_driver& driver)

YY_DECL;

class izna_driver
{
public:
	bool calc();

	int value(const std::string *name)
	{
		return values[*name];
	}

	void assign(const std::string *name, int num)
	{
		values[*name] = num;
	}

	void error(const std::string &m);

private:
	std::map<std::string, int> values;
};
#endif

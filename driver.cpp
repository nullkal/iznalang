#include <iostream>
#include "driver.h"
#include "parser.hh"

bool izna_driver::calc()
{
	yy::izna_parser parser(*this);

	int result = parser.parse();

	if (result != 0)
	{
		return false;
	}
	return true;
}

void izna_driver::error(const std::string &m)
{
	std::cerr << m << std::endl;
}

#include <iostream>
#include <cstring>
#include "parser.hh"

int main(int argc, char *argv[])
{
	char buf[] = "1 + 1";

	izna::parser_params params;
	params.input = buf;
	params.inputBegin = params.input;
	params.inputEnd   = params.input + strlen(buf);

	izna::parser parser(params);

	if (parser.parse() != 0)
	{
		std::cout << "Failed to parse the input file." << std::endl;
		return 1;
	}

	return 0;
}


#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace Token {
	// Token.type will be [0-255] if it is an unknown character
	enum Token_Type {
		TOK_EOF = -1,
		TOK_DEF = -2,
		TOK_EXTERN = -3,
		TOK_IDENTIFIER = -4,
		TOK_NUMBER = -5
	};

	extern double NumVal;
	extern std::string IdentifierName;
}
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Token.type will be [0-255] if it is an unknown character
enum Token_Type {
	TOK_EOF = -1,
	TOK_DEF = -2,
	TOK_EXTERN = -3,
	TOK_IDENTIFIER = -4,
	TOK_NUMBER = -5
};

class Token {
public:
	Token();
	Token(Token_Type t): type(t) { ; }

	Token_Type type;
	struct {
		union {
			double NumVal;
		};
		
		std::string IdentifierName;
	} header;
	
	int code_line, code_char;
	
	~Token() { };
private:
};
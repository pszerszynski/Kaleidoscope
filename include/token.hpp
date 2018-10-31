#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define OPERATOR_AMOUNT 10
enum OPERATOR { OP_ADD , OP_SUB , OP_MULT , OP_DIV , OP_L_THAN , OP_G_THAN , OP_LE_THAN , OP_GE_THAN , OP_AND , OP_OR , OP_NOT_OP , OP_NULL };
extern const std::string Operators[OPERATOR_AMOUNT];
// null-terminated string of recognised characters
extern const char* RecognisedChars;

namespace Token {
	// Token.type will be [0-255] if it is an unknown character
	enum Token_Type {
		TOK_EOF = -1,
		TOK_DEF = -2,
		TOK_EXTERN = -3,
		TOK_IDENTIFIER = -4,
		TOK_NUMBER = -5,
		TOK_OPERATOR = -6,
		
		TOK_IF = -7,
		TOK_THEN = -8,
		TOK_ELSE = -9
	};

	extern double NumVal;
	extern std::string IdentifierName;
	extern OPERATOR Operator;
}
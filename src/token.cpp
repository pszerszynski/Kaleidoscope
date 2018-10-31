#include "token.hpp"

// 1:1 correspondance with enum value and array index for that operator
const std::string Operators[OPERATOR_AMOUNT] = { "+", "-", "*", "/", "<", ">", "<=", ">=", "&&", "||" };
const char* RecognisedChars = "(,);";

double Token::NumVal;
std::string Token::IdentifierName;
OPERATOR Token::Operator;
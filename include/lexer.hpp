#pragma once

#include "token.hpp"

std::vector<Token> TokenizeFile(std::ifstream& f);
Token GetToken(std::ifstream& f);

Token TokenFromStr(std::string);
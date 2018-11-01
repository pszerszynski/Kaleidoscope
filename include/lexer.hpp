#pragma once

#include <string>

#include "token.hpp"
#include "parser.hpp"

extern bool LexerErr;

void LexerError(const std::string&);
int32_t GetToken(Parser& p);
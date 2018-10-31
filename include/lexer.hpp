#pragma once

#include <string>

#include "token.hpp"

extern bool LexerErr;

void LexerError(const std::string&);
int32_t GetToken(std::istream& f);
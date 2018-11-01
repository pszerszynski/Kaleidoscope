#pragma once

#include <vector>

#include "parser.hpp"
#include "std.hpp"

struct Variable;

struct Function {
	std::unique_ptr<FunctionAST> func;
};

struct Variable {
	std::string identifier;
	double value = 0.0;
};

namespace Interpreter {
	// set to true when error evaluating an expression
	// used to break out of error
	extern bool evalError;
	void LogError(std::string);
	
	void LoadSTD();
	
	extern std::vector<Variable> global_vars;
	
	// pointer to the vector with the current scopes variables
	extern std::unique_ptr<std::vector<Variable>> scopeVars;
	extern std::vector<Function> funcs;
};
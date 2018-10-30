#include "interpreter.hpp"

bool Interpreter::evalError = false;
std::vector<Variable> Interpreter::global_vars = std::vector<Variable>();

std::unique_ptr<std::vector<Variable>> Interpreter::scopeVars = nullptr;
std::vector<Function> Interpreter::funcs = std::vector<Function>();

void Interpreter::LogError(std::string str) {
	std::cerr << "LogError: " << str << std::endl;
	Interpreter::evalError = true;
}
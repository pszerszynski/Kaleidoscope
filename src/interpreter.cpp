#include "interpreter.hpp"

bool Interpreter::evalError = false;
std::vector<Variable> Interpreter::global_vars = std::vector<Variable>();

std::unique_ptr<std::vector<Variable>> Interpreter::scopeVars = nullptr;
std::vector<Function> Interpreter::funcs = std::vector<Function>();

void Interpreter::LogError(std::string str) {
	std::cerr << "LogError: " << str << std::endl;
	Interpreter::evalError = true;
}

void Interpreter::LoadSTD() {
	for (auto F : StandardLibrary) {
		Function func;
		func.func = std::make_unique<FunctionAST>(F);
		Interpreter::funcs.push_back(std::move(func));
	}
	
	StandardLibrary.clear();
}
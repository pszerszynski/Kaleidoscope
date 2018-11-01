#pragma once

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

struct ExternalFunction {
	ExternalFunction() = default;
	ExternalFunction(const std::string& _name, double (*_func)(std::vector<double>&), std::vector<std::string> _args):
		name(_name), func(_func), args(_args) { ; }
	
	std::string name;
	double (*func)(std::vector<double>&) = nullptr;
	std::vector<std::string> args;
};

extern std::vector<ExternalFunction> StandardLibrary;

void InitSTD();
#include "std.hpp"

std::vector<ExternalFunction> StandardLibrary = std::vector<ExternalFunction>();

namespace KaleSTD {
	double putchar(std::vector<double>& args) {
		std::cout << (char)args.at(0) << std::endl;
		return std::nan("");
	}

	double sin(std::vector<double>& args) {
		return std::sin(args.at(0));
	}
}

void InitSTD() {
	StandardLibrary.push_back(ExternalFunction("putchar", &KaleSTD::putchar, {"c"}));
	StandardLibrary.push_back(ExternalFunction("sin", &KaleSTD::sin, {"x"}));
}
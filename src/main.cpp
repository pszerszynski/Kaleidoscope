#include <iostream>
#include <fstream>

#include "interpreter.hpp"

// Top-Level parsing

void HandleDefinition(Parser& parser) {
	std::unique_ptr<FunctionAST> funcAST = parser.ParseDefinition();
	if (funcAST != nullptr) {
		Function func;
		func.func = std::move(funcAST);
		Interpreter::funcs.push_back(std::move(func));
	} else {
		// Skip token for error recovery.
		parser.GetNextToken();
	}
}

void HandleExtern(Parser& parser) {
	if (parser.ParseExtern()) {
		NULL;
	} else {
		// Skip token for error recovery.
		parser.GetNextToken();
	}
}

void HandleTopLevelExpression(Parser& parser) {
	// Evaluate a top-level expression into an anonymous function.
	std::unique_ptr<FunctionAST> topAST = parser.ParseTopLevelExpr();
	if (topAST != nullptr) {
		std::vector<double> args;
		double val = topAST->Call(args);
		std::cout << val << std::endl;
	} else {
		// Skip token for error recovery.
		parser.GetNextToken();
	}
	
	topAST.reset();
}

void MainLoop(Parser& parser) {
	while (true) {
		if (!parser.IsOpen())
			std::cerr << "ready> " << std::flush;
		
		switch (parser.CurrentTok) {
		case Token::TOK_EOF:
			return;
		case ';': // ignore top-level semicolons.
			parser.GetNextToken();
			break;
		case Token::TOK_DEF:
			HandleDefinition(parser);
			break;
		case Token::TOK_EXTERN:
			HandleExtern(parser);
			break;
		default:
			HandleTopLevelExpression(parser);
			break;
		}
	}
}

/// top ::= definition | external | expression | ';'
int main(int argc, char** argv) {
	InitSTD();
	Interpreter::LoadSTD();
	
	if (argc > 1) {
		Parser parser = Parser(std::string(argv[1]));
		if (!parser.IsOpen()) {
			std::cout << "Could not find file \"" << argv[1] << "\"" << std::endl;
		}
		
		parser.GetNextToken();
		MainLoop(parser);
	} else {
		Parser parser(std::cin);
		std::cerr << "ready> " << std::flush;
		parser.GetNextToken();
		MainLoop(parser);
	}
	
	return 0;
}
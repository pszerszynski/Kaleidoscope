#include "interpreter.hpp"

// Top-Level parsing

void HandleDefinition(Parser& parser) {
	std::unique_ptr<FunctionAST> funcAST = parser.ParseDefinition();
	if (funcAST != nullptr) {
		Function func;
		func.func = std::move(funcAST);
		Interpreter::funcs.push_back(std::move(func));
		fprintf(stderr, "Parsed a function definition.\n");
	} else {
		// Skip token for error recovery.
		parser.GetNextToken();
	}
}

void HandleExtern(Parser& parser) {
	if (parser.ParseExtern()) {
		fprintf(stderr, "Parsed an extern\n");
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
		std::cerr << "ready> " << std::flush;
	}
}

/// top ::= definition | external | expression | ';'
int main(int argc, char** argv) {
	Parser parser(std::cin);
	
	std::cerr << "ready> " << std::flush;
	parser.GetNextToken();
	
	MainLoop(parser);
	
	return 0;
}
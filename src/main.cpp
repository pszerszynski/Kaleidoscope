#include "parser.hpp"

// Top-Level parsing

static void HandleDefinition(Parser& parser) {
	if (parser.ParseDefinition()) {
		fprintf(stderr, "Parsed a function definition.\n");
	} else {
		// Skip token for error recovery.
		parser.GetNextToken();
	}
}

static void HandleExtern(Parser& parser) {
	if (parser.ParseExtern()) {
		fprintf(stderr, "Parsed an extern\n");
	} else {
		// Skip token for error recovery.
		parser.GetNextToken();
	}
}

static void HandleTopLevelExpression(Parser& parser) {
	// Evaluate a top-level expression into an anonymous function.
	if (parser.ParseTopLevelExpr()) {
		fprintf(stderr, "Parsed a top-level expr\n");
	} else {
		// Skip token for error recovery.
		parser.GetNextToken();
	}
}

/// top ::= definition | external | expression | ';'
int main(int argc, char** argv) {
	Parser parser(std::cin);
	
	while (true) {
		fprintf(stderr, "ready> ");
		switch (parser.CurrentTok) {
		case Token::TOK_EOF:
			return 0;
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
	
	return 0;
}
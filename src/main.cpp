#include "lexer.hpp"

int main(int argc, char** argv) {
	std::ifstream file;
	if (argc < 2) {
		std::cerr << "ERROR: No input given" << std::endl;
		return -1;
	}
	file.open(argv[1]);
	if (!file.is_open()) {
		std::cerr << "ERROR: File cannot be opened" << std::endl;
		return -1;
	}
	
	std::vector<Token> tokens = TokenizeFile(file);
	
	for (auto T : tokens) {
		std::cout << "[TOKEN_TYPE:";
		switch (T.type) {
		case TOK_DEF:
			std::cout << "TOK_DEF";
			break;
		case TOK_EOF:
			std::cout << "TOK_EOF";
			break;
		case TOK_EXTERN:
			std::cout << "TOK_EXTERN";
			break;
		case TOK_IDENTIFIER:
			std::cout << "TOK_IDENTIFIER " << T.header.IdentifierName;
			break;
		case TOK_NUMBER:
			std::cout << "TOK_NUMBER " << T.header.NumVal;
			break;
		default:
			std::cout << "CHARACTER \"" << (char)T.type << '\"';
		}
		std::cout << "]" << std::endl;
	}
	
	file.close();
	
	return 0;
}
#include "lexer.hpp"

std::vector<Token> TokenizeFile(std::ifstream& f) {
	std::vector<Token> tokens;
	
	while (true) {
		Token t = GetToken(f);
		tokens.push_back(t);
		if (t.type == TOK_EOF)
			break;
	}
	
	return tokens;
}

Token GetToken(std::ifstream& f) {	
	static char LastChar = ' ';
	
	while (isspace(LastChar))
		LastChar = f.get();
		
	if (isalpha(LastChar)) {
		std::string IdentifierStr = "";
		
		IdentifierStr = LastChar;
		while (isalnum( (LastChar = f.get()) )) {
			IdentifierStr += LastChar;
		}
		
		if (IdentifierStr == "def")
			return Token(TOK_DEF);
		else if (IdentifierStr == "extern")
			return Token(TOK_EXTERN);
		
		Token result = Token(TOK_IDENTIFIER);
		result.header.IdentifierName = IdentifierStr;
		return result;
	}
	
	if (isdigit(LastChar) || LastChar == '.') {
		std::string NumStr;
		
		while (isdigit(LastChar) || LastChar == '.') {
			NumStr += LastChar;
			LastChar = f.get();
		}
		
		Token result = Token(TOK_NUMBER);
		result.header.NumVal = strtod(NumStr.c_str(), 0);
		return result;
	}
	
	if (LastChar == '#') {
		do
			LastChar = f.get();
		while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
		
		if (LastChar == EOF)
			return Token(TOK_EOF);
	}
	
	int ThisChar = LastChar;
	LastChar = f.get();
	return Token((Token_Type)ThisChar);
}

Token TokenFromStr(std::string) {
	return Token(TOK_EOF);
}
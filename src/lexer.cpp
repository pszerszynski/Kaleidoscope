#include "lexer.hpp"

int32_t GetToken(std::istream& f) {	
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
			return Token::TOK_DEF;
		else if (IdentifierStr == "extern")
			return Token::TOK_EXTERN;
		
		Token::IdentifierName = IdentifierStr;
		return Token::TOK_IDENTIFIER;
	}
	
	if (isdigit(LastChar) || LastChar == '.') {
		std::string NumStr;
		
		while (isdigit(LastChar) || LastChar == '.') {
			NumStr += LastChar;
			LastChar = f.get();
		}
		
		Token::NumVal = strtod(NumStr.c_str(), 0);
		return Token::TOK_NUMBER;
	}
	
	if (LastChar == '#') {
		do
			LastChar = f.get();
		while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
		
		if (LastChar == EOF)
			return Token::TOK_EOF;
	}
	
	int32_t ThisChar = LastChar;
	LastChar = f.get();
	return ThisChar;
}
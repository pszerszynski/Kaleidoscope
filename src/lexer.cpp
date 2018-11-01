#include "lexer.hpp"

bool LexerErr = false;

void LexerError(const std::string& str) {
	LexerErr = true;
	std::cerr << "Lexer LogError: " << str << std::endl;
}


#define RETURN_TOKEN(tok) LastToken = tok; return tok
int32_t GetToken(Parser& p) {
	static int LastToken = Token::TOK_EOF;
	static int LastChar = ' ';
	
	Token::Operator = OPERATOR::OP_NOT_OP;
	
	while (isspace(LastChar))
		LastChar = p.GetNextChar();
		
	if (LastChar == EOF) {
		RETURN_TOKEN(Token::TOK_EOF);
	}
		
	if (isalpha(LastChar)) {
		std::string IdentifierStr = "";
		
		IdentifierStr = LastChar;
		while (isalnum( (LastChar = p.GetNextChar()) )) {
			IdentifierStr += LastChar;
		}
		
		if (IdentifierStr == "def") {
			RETURN_TOKEN(Token::TOK_DEF);
		} else if (IdentifierStr == "if") {
			RETURN_TOKEN(Token::TOK_IF);
		} else if (IdentifierStr == "else") {
			Token::Operator = OPERATOR::OP_NULL;
			RETURN_TOKEN(Token::TOK_ELSE);
		} else if (IdentifierStr == "then") {
			RETURN_TOKEN(Token::TOK_THEN);
		} else if (IdentifierStr == "extern") {
			RETURN_TOKEN(Token::TOK_EXTERN);
		}
		
		Token::IdentifierName = IdentifierStr;
		RETURN_TOKEN(Token::TOK_IDENTIFIER);
	}
	
	if (isdigit(LastChar) || LastChar == '.' || (LastToken != Token::TOK_NUMBER &&
						     LastToken != Token::TOK_IDENTIFIER && LastToken != ')' && (LastChar == '-'))) {
		std::string NumStr;
		
		while (true) {
			NumStr += LastChar;
			LastChar = p.GetNextChar();
			if (isdigit(LastChar) || LastChar == '.')
				continue;
			break;
		}
		
		Token::NumVal = strtod(NumStr.c_str(), 0);
		RETURN_TOKEN(Token::TOK_NUMBER);
	}
	
	if (LastChar == '#') {
		do
			LastChar = p.GetNextChar();
		while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
	}
	
	for (char* c = (char*)RecognisedChars; *c; ++c) {
		if (*c == LastChar) {
			LastChar = p.GetNextChar();
			RETURN_TOKEN(*c);
		}
	}
	
	std::string temp = std::string(1, LastChar);
	
	while (true) {
		LastChar = p.GetNextChar();
		if (isalnum(LastChar) || isspace(LastChar)) {
			break;
		}
		
		bool b = false;
		const char* breakChars = "-.()";
		for (char* c = (char*)breakChars; *c; ++c) {
			if (*c == LastChar) {
				b = true;
				break;
			}
		}
		
		if (b)
			break;
		
		temp += LastChar;
	}
	
	bool found = false;
	for (std::size_t i = 0; i < OPERATOR_AMOUNT; ++i) {
		if (Operators[i] == temp) {
			Token::Operator = (OPERATOR)i;
			RETURN_TOKEN(Token::TOK_OPERATOR);
		}
	}
	
	if (temp.size() > 1) {
		LexerError("unrecognised operator \"" + temp + "\"");
		return 0;
	}
	
	LexerError("unexpected character \"" + std::string(1, temp[0]) + "\"");
	return 0;
}
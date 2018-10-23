#include "parser.hpp"

int Parser::GetNextToken() {
	return this->CurrentTok = GetToken(this->file);
}

std::unique_ptr<ExprAST> Parser::LogError(const std::string& str) {
	std::cerr << "LogError: " << str << std::endl;
	return nullptr;
}
	
std::unique_ptr<PrototypeAST> Parser::LogErrorP(const std::string& str) {
	LogError(str);
	return nullptr;
}

int Parser::GetTokenPrecedence() {
	if (!isascii(this->CurrentTok))
		return -1;
		
	int TokPrec = this->BinopPrecedence[this->CurrentTok];
	if (TokPrec <= 0) return -1;
	return TokPrec;
}

void Parser::InstallPrecedence() {
	BinopPrecedence['<'] = 10;
	BinopPrecedence['+'] = 20;
	BinopPrecedence['-'] = 20;
	BinopPrecedence['*'] = 40;
}

// numberexpr ::= number
std::unique_ptr<ExprAST> Parser::ParseNumberExpr() {
	auto result = std::make_unique<NumberExprAST>(Token::NumVal);
	this->GetNextToken();
	return std::move(result);
}

// parenexpr ::= '(' expr ')'
std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
	this->GetNextToken();
	auto V = ParseExpression();
	if (!V)
		return nullptr;
		
	if (this->CurrentTok != ')') {
		return LogError("expected \')\'");
	}
	
	this->GetNextToken();
	return V;
}

// identifierexpr
// 	::= identifier
//	::= identifier '(' expr ')'
std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr() {
	std::string idName = Token::IdentifierName;
	
	this->GetNextToken();
	
	if (this->CurrentTok != '(') // simple variable ref
		return std::make_unique<VariableExprAST>(idName);
		
	// Call function
	this->GetNextToken(); // eat (
	std::vector<std::unique_ptr<ExprAST>> args;
	if (this->CurrentTok != ')') {
		while (1) {
			if (auto arg = ParseExpression())
				args.push_back(std::move(arg));
			else
				return nullptr;
				
			if (this->CurrentTok == ')')
				break;
				
			if (this->CurrentTok != ',')
				return LogError("expected \')\' or \',\' in argument list");
				
			this->GetNextToken();
		}
	}
	
	// eat the )
	this->GetNextToken();
	
	return std::make_unique<CallExprAST>(idName, std::move(args));
}

// primary
//	::= indentifierexpr
//	::= numberexpr
//	::= parenexpr
std::unique_ptr<ExprAST> Parser::ParsePrimary() {
	switch (this->CurrentTok) {
	default:
		return LogError("unknown token when expecting an expression");
	case Token::Token_Type::TOK_IDENTIFIER:
		return ParseIdentifierExpr();
	case Token::Token_Type::TOK_NUMBER:
		return ParseNumberExpr();
	case '(':
		return ParseParenExpr();
	}
}

// expression
//	::= primary binop_rhs
std::unique_ptr<ExprAST> Parser::ParseExpression() {
	auto LHS = this->ParsePrimary();
	if (!LHS)
		return nullptr;
		
	return this->ParseBinOpRHS(0, std::move(LHS));
}

// binorp_rhs
//	::= ('+' primary)*
std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
	// if this is a binop, find its precedence
	
	while (1) {
		int TokPrec = GetTokenPrecedence();
		
		if (TokPrec < ExprPrec)
			return LHS;
			
		int BinOp = this->CurrentTok;
		this->GetNextToken(); // eat binop
		
		// parse the primary expression after the binary operator
		auto RHS = this->ParsePrimary();
		if (!RHS)
			return nullptr;
			
		int NextPrec = this->GetTokenPrecedence();
		if (TokPrec < NextPrec) {
			RHS = this->ParseBinOpRHS(TokPrec+1, std::move(RHS));
			if (!RHS)
				return nullptr;
		}
		
		// Merge LHS/RHS
		LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
	} // loop around to the top of the while loop
}

// prototype
//	::= id '(' id* ')'
std::unique_ptr<PrototypeAST> Parser::ParsePrototype() {
	if (this->CurrentTok != Token::TOK_IDENTIFIER)
		return LogErrorP("Expected function name in prototype");
		
	std::string FnName = Token::IdentifierName;
	this->GetNextToken();
	
	if (this->CurrentTok != '(')
		return LogErrorP("Expected \'(\' in prototype");
		
	// read the list of argument 
	std::vector<std::string> argNames;
	while (this->GetNextToken() == Token::TOK_IDENTIFIER)
		argNames.push_back(Token::IdentifierName);
	if (this->CurrentTok != ')')
		return LogErrorP("Expected \')\' in prototype");
		
	// success
	this->GetNextToken(); // eat )
	
	return std::make_unique<PrototypeAST>(FnName, std::move(argNames));
}

// definition ::= 'def' prototype expression
std::unique_ptr<FunctionAST> Parser::ParseDefinition() {
	this->GetNextToken();
	auto proto = this->ParsePrototype();
	if (!proto)
		return nullptr;
		
	if (auto E = this->ParseExpression())
		return std::make_unique<FunctionAST>(std::move(proto), std::move(E));
	return nullptr;
}

// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> Parser::ParseExtern() {
	this->GetNextToken();
	return this->ParsePrototype();
}

// toplevelexpr ::= expression
std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
	if (auto E = this->ParseExpression()) {
		auto proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
		return std::make_unique<FunctionAST>(std::move(proto), std::move(E));
	}
	return nullptr;
}
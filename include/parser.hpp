#pragma once

#include <map>
#include <iostream>

#include "lexer.hpp"
#include "ast.hpp"

class Parser {
public:
	Parser(std::istream& istr): file(istr) { this->InstallPrecedence(); }
	
	int GetNextToken();
	int CurrentTok;

	std::unique_ptr<ExprAST> LogError(const std::string& str);
	std::unique_ptr<PrototypeAST> LogErrorP(const std::string& str);
	
	std::unique_ptr<ExprAST> ParseExpression();
	std::unique_ptr<ExprAST> ParseNumberExpr();
	std::unique_ptr<ExprAST> ParseParenExpr();
	std::unique_ptr<ExprAST> ParseIdentifierExpr();
	std::unique_ptr<ExprAST> ParsePrimary();
	std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
	
	std::unique_ptr<PrototypeAST> ParsePrototype();
	std::unique_ptr<FunctionAST> ParseDefinition();
	std::unique_ptr<PrototypeAST> ParseExtern();
	std::unique_ptr<FunctionAST> ParseTopLevelExpr();
private:
	std::istream& file;
	
	// 1 is the lowest precedence for binary operators
	std::map<char, int> BinopPrecedence;
	int GetTokenPrecedence();
	void InstallPrecedence();
};
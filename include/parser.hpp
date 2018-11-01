#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include <string>

class Parser;

#include "lexer.hpp"
#include "ast.hpp"

class Parser {
public:
	Parser(std::istream& istr): stream(std::cin), file(std::ifstream()), is_file(false) { ; this->InstallPrecedence(); }
	Parser(const std::string& filename): stream(std::cin), file(std::ifstream(filename)), is_file(true) { this->InstallPrecedence(); }
	
	bool IsOpen();
	
	char GetNextChar();
	int GetNextToken();
	int CurrentTok = 0;

	std::unique_ptr<ExprAST> LogError(const std::string& str);
	std::unique_ptr<PrototypeAST> LogErrorP(const std::string& str);
	
	std::unique_ptr<ExprAST> ParseExpression();
	std::unique_ptr<ExprAST> ParseNumberExpr();
	std::unique_ptr<ExprAST> ParseParenExpr();
	std::unique_ptr<ExprAST> ParseIdentifierExpr();
	std::unique_ptr<ExprAST> ParsePrimary();
	std::unique_ptr<ExprAST> ParseIfElse();
	std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
	
	std::unique_ptr<PrototypeAST> ParsePrototype();
	std::unique_ptr<FunctionAST> ParseDefinition();
	std::unique_ptr<PrototypeAST> ParseExtern();
	std::unique_ptr<FunctionAST> ParseTopLevelExpr();
	
	~Parser();
private:
	std::istream& stream;
	std::ifstream file;
	bool is_file;
	
	// 1 is the lowest precedence for binary operators
	std::map<OPERATOR, int> BinopPrecedence;
	int GetTokenPrecedence();
	void InstallPrecedence();
};
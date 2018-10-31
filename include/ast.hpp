#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cmath>

// base class for all expr nodes
class ExprAST {
public:
	virtual double Evaluate() = 0;
	virtual ~ExprAST() { ; }
};

// expr class for numeric literals such as "2.4"
class NumberExprAST : public ExprAST {
private:
	double val = 0.0;
public:
	double Evaluate();
	NumberExprAST(double Val) : val(Val) { ; }
};

// expr class for a variable like "a"
class VariableExprAST : public ExprAST {
private:
	std::string name;
public:
	double Evaluate();
	VariableExprAST(const std::string& N) : name(N) { ; }
};

// expr class for a binary operator
class BinaryExprAST : public ExprAST {
private:
	OPERATOR op;
	std::unique_ptr<ExprAST> left_expr, right_expr;
public:
	double Evaluate();
	BinaryExprAST(OPERATOR _op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
		: op(_op), left_expr(std::move(LHS)), right_expr(std::move(RHS)) { ; }
};

// expr class for a function call
class CallExprAST : public ExprAST {
private:
	std::string callee;
	std::vector<std::unique_ptr<ExprAST>> args;
public:
	const std::vector<std::unique_ptr<ExprAST>>& GetArgs();

	double Evaluate();
	CallExprAST(const std::string& callee,
		std::vector<std::unique_ptr<ExprAST>> args)
		: callee(callee), args(std::move(args)) { ; }
};

// expr class for an if-else statement
class IfElseExprAST : public ExprAST {
private:
	std::unique_ptr<ExprAST> condition, body, else_body;
public:
	double Evaluate();
	IfElseExprAST(std::unique_ptr<ExprAST> COND, std::unique_ptr<ExprAST> BODY, std::unique_ptr<ExprAST> ELSE_BODY)
		: condition(std::move(COND)), body(std::move(BODY)), else_body(std::move(ELSE_BODY)) { ; }
};

class PrototypeAST {
private:
	std::string name;
	std::vector<std::string> args;
public:
	const std::string& GetName();
	const std::vector<std::string>& GetArgs();

	PrototypeAST(const std::string& name, std::vector<std::string> args)
		: name(name), args(std::move(args)) { ; }
};

class FunctionAST {
private:
	std::unique_ptr<PrototypeAST> proto;
	std::unique_ptr<ExprAST> body;
public:
	const std::string& GetName();
	double Call(std::vector<double>& callArgs);

	FunctionAST(std::unique_ptr<PrototypeAST> proto,
		std::unique_ptr<ExprAST> body)
		: proto(std::move(proto)), body(std::move(body)) { ; }
};

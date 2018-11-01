#include "interpreter.hpp"

/*
	Get functions
*/

const std::string& PrototypeAST::GetName() {
	return this->name;
}
const std::vector<std::string>& PrototypeAST::GetArgs() {
	return this->args;
}

const std::string& FunctionAST::GetName() {
	return this->proto->GetName();
}

const std::vector<std::unique_ptr<ExprAST>>& CallExprAST::GetArgs() {
	return this->args;
}

/*
	Evaluation functions
*/

double NumberExprAST::Evaluate() {
	return this->val;
}

double BinaryExprAST::Evaluate() {
	double LHS = this->left_expr->Evaluate(),
	       RHS = this->right_expr->Evaluate();
	if ( (LHS == nan("")) || (RHS == nan("")) )
		return nan("");
	       
	switch (this->op) {
	case OPERATOR::OP_ADD:
		return LHS + RHS;
		break;
	case OPERATOR::OP_SUB:
		return LHS - RHS;
		break;
	case OPERATOR::OP_MULT:
		return LHS * RHS;
		break;
	case OPERATOR::OP_DIV:
		return LHS / RHS;
		break;
	case OPERATOR::OP_G_THAN:
		return LHS > RHS ? 1.0 : 0.0;
		break;
	case OPERATOR::OP_L_THAN:
		return LHS < RHS ? 1.0 : 0.0;
		break;
	case OPERATOR::OP_GE_THAN:
		return LHS >= RHS ? 1.0 : 0.0;
		break;
	case OPERATOR::OP_LE_THAN:
		return LHS >= RHS ? 1.0 : 0.0;
		break;
	case OPERATOR::OP_OR:
		return LHS || RHS ? 1.0 : 0.0;
		break;
	case OPERATOR::OP_AND:
		return LHS && RHS ? 1.0 : 0.0;
		break;
	}
}

double VariableExprAST::Evaluate() {
	double val;
	if (Interpreter::scopeVars != nullptr) {
		for (auto V : *Interpreter::scopeVars) {
			if (V.identifier == this->name)
				return V.value;
		}
	}
	
	for (auto global_V : Interpreter::global_vars) {
		if (global_V.identifier == this->name)
			return global_V.value;
	}
	
	Interpreter::LogError("variable with identifier \"" + this->name + "\" cannot be found");
	return std::nan("");
}

double CallExprAST::Evaluate() {
	for (auto F = Interpreter::funcs.begin(); F < Interpreter::funcs.end(); ++F) {
		if (F->func->GetName() == this->callee) {
			std::vector<double> callArgs;
			for (std::size_t i = 0; i != this->args.size(); ++i) {
				callArgs.push_back(this->args.at(i)->Evaluate());
			}
			
			bool saveScope = true;
			if (Interpreter::scopeVars == nullptr)
				saveScope = false;
			
			std::unique_ptr<std::vector<Variable>> curScope;
			if (saveScope)
				curScope = std::move(Interpreter::scopeVars);
			
			double val = F->func->Call(callArgs);
			
			if (saveScope)
				Interpreter::scopeVars = std::move(curScope);
			else
				Interpreter::scopeVars.reset();
			
			return val;
		}
	}
	
	Interpreter::LogError("function \"" + this->callee + "\" doesn't exist");
	return std::nan("");
};

double IfElseExprAST::Evaluate() {
	double val;
	if (this->condition->Evaluate() != 0.0) {
		val = this->body->Evaluate();
	} else {
		if (this->else_body != nullptr)
			val = this->else_body->Evaluate();
		else
			val = std::nan("");
	}
	
	return val;
}

double FunctionAST::Call(std::vector<double>& callArgs) {
	if (this->is_extern)
		return this->CallExternalFunction(callArgs);
	
	if (callArgs.size() != this->proto->GetArgs().size()) {
		std::string errorStr = this->proto->GetName() + "(";
		for (auto a = this->proto->GetArgs().begin(); a != this->proto->GetArgs().end(); ++a) {
			errorStr += *a;
			if (a != this->proto->GetArgs().end() - 1)
				errorStr += " ";
		}
		errorStr += ") expects " + std::to_string(this->proto->GetArgs().size()) + " arguments, got "
			+ std::to_string(callArgs.size());
		Interpreter::LogError(errorStr);
		return std::nan("");
	}
	
	std::vector<Variable> scopeVars;
	for (std::size_t i = 0; i < callArgs.size(); ++i) {
		Variable var;
		var.value = callArgs.at(i);
		var.identifier = this->proto->GetArgs().at(i);
		scopeVars.push_back(var);
	}
	
	Interpreter::scopeVars = std::make_unique<std::vector<Variable>>(std::move(scopeVars));
	
	double returnVal = this->body->Evaluate();
	
	Interpreter::scopeVars.reset();
	return returnVal;
}

double FunctionAST::CallExternalFunction(std::vector<double>& callArgs) {
	if (callArgs.size() != this->proto->GetArgs().size()) {
		std::string errorStr = this->proto->GetName() + "(";
		for (auto a = this->proto->GetArgs().begin(); a != this->proto->GetArgs().end(); ++a) {
			errorStr += *a;
			if (a != this->proto->GetArgs().end() - 1)
				errorStr += " ";
		}
		errorStr += ") expects " + std::to_string(this->proto->GetArgs().size()) + " arguments, got "
			+ std::to_string(callArgs.size());
		Interpreter::LogError(errorStr);
		return std::nan("");
	}
	
	return this->external_func(callArgs);
}

// Simple Expression Evaluation 
// This program will evaluate simple arithmetic expressions
// represented as a linked-list of tokens.  Keyboard input
// will be accepted into a string, which will be converted
// into that linked list.
//
// If the first symbol in the input string is an operator,
// then the value of the previous expression will be taken
// as an implicit first operand.
//
// The expressions may consist of the following:
// -- integer values (which may have multiple digits)
// -- simple arithmetic opers ( +, -, *, /, % )
// -- matched parentheses for grouping

// This implementation consists of a set of mutually-recursive 
// functions. which will track the structure of the expression.
//
// A sum expression is the sum or difference of one or more products.
// A product expression is the product or quotient of one or more factors.
// A factor may be a number or a parenthesized sum expression.

#include <iostream>
#include <string>
using namespace std;
#include "tokenlist.h"
#include "exprtree.h"
#include "funmap.h"

ExprNode * evaluateAssignment(ListIterator &, TokenList &, FunctionDef &);
ExprNode * evaluateSum(ListIterator &, TokenList &, FunctionDef &);
ExprNode * evaluateProduct(ListIterator &, TokenList &, FunctionDef &);
ExprNode * evaluateFactor(ListIterator &, TokenList &, FunctionDef &);
ExprNode * evaluateConditional(ListIterator &, TokenList &, FunctionDef &);
ExprNode * evaluateRelational(ListIterator &, TokenList &, FunctionDef &);
bool checkForFunction(ListIterator iter);

// Evaluate
// Begins to tokenize the string, and then recursively evaluates it.
// Parameters:
// 	str	(input char array)	// string to evaluate
// Pre-condition:  str must be a valid integer arithmetic expression
//     including matching parentheses.  The only exception to this 
//     pre-condition is the possible absence of a value at the front
//     which will be filled in by the return value from a previous call
int evaluate(const char str[], VarTree &vars, FunctionDef &funs)
{
	TokenList list(str);
	ListIterator iter = list.begin();
	TokenList postfix = TokenList();	

	return evaluateAssignment(iter, list, funs)->evaluate(vars);
}

//Evaluate Assignment
//Evaluates an assignment using the other functions first and then assigning a value to a variable if present
ExprNode * evaluateAssignment(ListIterator &iter, TokenList &list, FunctionDef &funs)
{
	string oper = "";
	ExprNode *left = NULL, *right, *curr = NULL;
	FunDef newFunction; //in case a new function is defined

	int counter = 0; //keeps track of number of parameters in array

	if (iter.token().tokenText() == "deffn")
	{
		iter.advance(); //get past deffn
		newFunction.locals = new VarTree();
		newFunction.name = iter.token().tokenText();
		iter.advance(); //get past name of function
		iter.advance(); //get past '('
		for (int i = 0; iter.tokenChar() != ')'; i++)
		{
			newFunction.parameter[i] = iter.token().tokenText();
			iter.advance();
			if (iter.token().tokenChar() == ',')
				iter.advance();
			counter++;
		}
		while (counter < 10)
		{
			newFunction.parameter[counter] == "";
			counter++;
		}
		iter.advance(); //get past ')'
		iter.advance(); //get past '='
		newFunction.functionBody = evaluateAssignment(iter, list, funs);
		funs[newFunction.name] = newFunction;
	}
	
	if (counter == 10)
		left = newFunction.functionBody;
	else
		left = evaluateConditional(iter, list, funs);
	if (iter != list.end())                          //This is to make sure null characters are not attempted to be accessed in all functions
		oper = iter.token().tokenText();
	else
		oper = "";
	while (oper == "=")
	{
		iter.advance();	// get past the operator
		right = evaluateAssignment(iter, list, funs);
		curr = new Operation(left, oper, right);
		if (iter != list.end())
			oper = iter.token().tokenText();
		else
			oper = "";
		left = curr;
	}

	if (curr == NULL)
		return left;
	else
		return curr;
}

//Evaluate Conditional
//Handles conditional statements by evaluating the test cause, then evaluating the true and false cases, and returning the correct result
ExprNode * evaluateConditional(ListIterator &iter, TokenList &list, FunctionDef &funs)
{
	string oper = "";			// possible operation
	ExprNode *root, *test, *trueCase, *falseCase;

	test = evaluateRelational(iter, list, funs);
	root = NULL;
	if (iter != list.end())
		oper = iter.token().tokenText();
	else
		oper = "";

	while (oper == "?")
	{
		iter.advance();
		trueCase = evaluateRelational(iter, list, funs);
		iter.advance();
		falseCase = evaluateRelational(iter, list, funs);
		root = new Conditional(test, trueCase, falseCase);
		if (iter != list.end())
			oper = iter.token().tokenText();
		else
			oper = "";
		test = root;
	}

	if (root == NULL)
		return test;
	else
		return root;
}

//Evaluate Relational
//Handles relational statements, comparing two evaluated statements 
ExprNode * evaluateRelational(ListIterator &iter, TokenList &list, FunctionDef &funs)
{
	string oper = "";			// possible operation
	ExprNode *left, *right, *curr = NULL;

	left = evaluateSum(iter, list, funs);
	if (iter != list.end())
		oper = iter.token().tokenText();
	else
		oper = "";
	while (oper == "<" || oper == ">" || oper == "<=" || oper == ">=" || oper == "==" || oper == "!=")
	{
		iter.advance();	// get past the operator
		right = evaluateSum(iter, list, funs);
		curr = new Operation(left, oper, right);
		if (iter != list.end())
			oper = iter.token().tokenText();
		else
			oper = "";
		left = curr;
	}

	if (curr == NULL)
		return left;
	else
		return curr;
}

// EvaluateSum
// Evaluates a sum expression: the sum or difference of one or more products
// There may be the possibility of a leading - that would be implicitly
// subtracting the first product from zero.
ExprNode * evaluateSum(ListIterator &iter, TokenList &list, FunctionDef &funs)
{
	string oper = "";			// possible operation
	ExprNode *left, *right, *curr = NULL;
	
	left = evaluateProduct(iter, list, funs);
	if (iter != list.end())
		oper = iter.token().tokenText();
	else
		oper = "";
	while (oper == "+" || oper == "-")
	{
		iter.advance();	// get past the operator
		right = evaluateProduct(iter, list, funs);
		curr = new Operation(left, oper, right);
		if (iter != list.end())
			oper = iter.token().tokenText();
		else
			oper = "";
		left = curr;
	}
	
	if (curr == NULL)
		return left;
	else
		return curr;
}


// EvaluateProduct
// Evaluates a product expression: the product or quotient of factors
ExprNode * evaluateProduct(ListIterator &iter, TokenList &list, FunctionDef &funs)
{
	string oper = "";		// possible operation
	ExprNode *left , *right = NULL, *curr = NULL;

	if (iter.tokenChar() == '-')	// if negative
	{
		iter.advance();			//    go past the negative sign
		right = evaluateProduct(iter, list, funs);	//       build a tree 
		left = new Operation(new Value(0), "-", right);
	}
	else
		left = evaluateFactor(iter, list, funs);

	if (iter != list.end())
		oper = iter.token().tokenText();
	else
		oper = "";
	while (oper == "*" || oper == "/" || oper == "%")
	{
		iter.advance();	// get past the operator
		right = evaluateFactor(iter, list, funs);
		curr = new Operation(left, oper, right);
		if (iter != list.end())
			oper = iter.token().tokenText();
		else
			oper = "";
		left = curr;
	}

	if (curr == NULL)
		return left;
	else 
		return curr;
}

// Evaluate Factor
// A factor may either be a single-digit number 
// or a parenthsized expression.
ExprNode * evaluateFactor(ListIterator &iter, TokenList &list, FunctionDef &funs)
{
	ExprNode * temp;

	if (iter.currentIsInteger())
	{
		Value *number = new Value(iter.integerValue());

		iter.advance();		// get past the digit

		return static_cast<ExprNode *>(number);
	}
	else if (iter.tokenChar() == '(')
	{
		iter.advance();		// go past assumed (
		temp = evaluateAssignment(iter, list, funs);
		iter.advance();		// go past assumed )
		return temp;
	}
	else if (checkForFunction(iter)) //For handling functions
	{
		string functionName = iter.token().tokenText();
		ExprNode *parameters[10];
		int count = 0; //keep track of number of parameters
		iter.advance(); //get past function name
		iter.advance(); //get past '('
		for (int i = 0; iter.tokenChar() != ')' ; i++)
		{
			if (iter.tokenChar() == ',')
			{
				iter.advance();
				i--; //so that a function parameter isn't skipped when it goes through the loop again
			}
			else
			{
				parameters[i] = evaluateAssignment(iter, list, funs);
				//iter.advance();
				count++;
			}
		}

		Function *fun = new Function(functionName, parameters, count, &funs);

		iter.advance(); //get past ')'

		return fun;
	}
	else //Handles variables
	{
		Variable *var = new Variable(iter.token().tokenText());

		iter.advance();

		return static_cast<ExprNode *>(var);
	}
	
}

bool checkForFunction(ListIterator iter)
{
	iter.advance();
	if (iter.tokenChar() == '(')
		return true;
	else
		return false;
}
// Expression Tree Implementation File
// These functions manage an expression tree that describes
// arithmetic expressions using integer variables and values.
// Some functions construct a tree from a character string that
// represents the expression, and then the tree can be traversed
// and evaluated.
#include <iostream>
#include <sstream>
using namespace std;
#include "exprtree.h"
#include "tokenlist.h"
#include "funmap.h"

// Outputting any tree node will simply output its string version
ostream& operator<<(ostream &stream, const ExprNode &e)
{
	return stream << e.toString();
}

// A Value is just an integer value -- easy to evaluate
// Unfortunately, the string class does not have a constructor for it
string Value::toString() const
{
	ostringstream convert;	// output string stream
	convert << value;		// 'output' into the stream
	return convert.str();	// and extract its string equivalent
}

int Value::evaluate(VarTree &v) const
{
	return value;
}

//  A variable is just an alphabetic string -- easy to display
//  TO evaluate, would need to look it up in the data structure
string Variable::toString() const
{
	return name;
}

int Variable::evaluate(VarTree &v) const
{
	return v.lookup(name);
}

string Operation::toString() const
{
	return oper;
}

//Handles each different operation
int Operation::evaluate(VarTree &v) const
{
	if (oper == "+")
		return left->evaluate(v) + right->evaluate(v);
	else if (oper == "-")
		return left->evaluate(v) - right->evaluate(v);
	else if (oper == "*")
		return left->evaluate(v) * right->evaluate(v);
	else if (oper == "/")
		return left->evaluate(v) / right->evaluate(v);
	else if (oper == "%")
		return left->evaluate(v) % right->evaluate(v);
	else if (oper == "#")
		return -left->evaluate(v);
	else if (oper == "=")
	{
		int number = right->evaluate(v);
		v.assign(left->toString(), number);
		return number;
	}
	else if (oper == ">")
		return (left->evaluate(v) > right->evaluate(v));
	else if (oper == "<")
		return (left->evaluate(v) < right->evaluate(v));
	else if (oper == ">=")
		return (left->evaluate(v) >= right->evaluate(v));
	else if (oper == "<=")
		return (left->evaluate(v) <= right->evaluate(v));
	else if (oper == "==")
		return (left->evaluate(v) == right->evaluate(v));
	else if (oper == "!=")
		return (left->evaluate(v) != right->evaluate(v));
	else
		cout << "Non supported operator used";
		
	return 0;
}

string Conditional::toString() const
{
	return test->toString() + "?" + trueCase->toString() + ":" + falseCase->toString();
}

//If the condition ends up being true, return the true case of the test and if not return the false case
int Conditional::evaluate(VarTree &v) const
{
	if (test->evaluate(v))
		return trueCase->evaluate(v);
	else
		return falseCase->evaluate(v);
}

string Function::toString() const
{
	return name + "(";
}

int Function::evaluate(VarTree &v) const
{
	for (int i = 0; parameter[i] != NULL; i++)
	{
		(*funs)[name].locals->assign((*funs)[name].parameter[i], parameter[i]->evaluate(v));
	}
	
	return (*funs)[name].functionBody->evaluate(*(*funs)[name].locals);
}
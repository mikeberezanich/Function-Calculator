// Token List Implementation file
//
// This tokenizer will scan a character string representing
// an expression, and will return a list of tokens.
// --- only integers and operators are supported (no names)

// The standard C library has some useful functions for us
#include <string.h>
#include <ctype.h>

// And to get the definition of a token:
#include "tokenlist.h"

// TokenList constructor
// converts a character string into a list of tokens
// Parameter:
// 	expr	(input char pointer)	// string to examine
// Pre-condition:  str may not be a null pointer
//     and is assumed to actually point at a valid expression.
TokenList::TokenList( const char expr[])
{
	int position = 0;
	string variable = ""; //A blank string for variable use
	string oper = ""; //A blank string for operator use
	head = NULL;
	tail = NULL;

	while (expr[position] == ' ')    //To get past spaces
		++position;
	
	while (expr[position] != '\0')   //Go until end of expression
	{
		if (isdigit(expr[position]))                 //Adds integers onto tokenlist
		{
			Token t(atoi(&expr[position]));
			if (empty())
				push_front(t);
			else
				push_back(t);
			while (isdigit(expr[position]))
				++position;
		}
		else if (ispunct(expr[position])) //Adds operators onto tokenlist
		{
			oper += expr[position];
			if (expr[position + 1] == '=')  //This makes sure the multiple char operations <=, >=, !=, and == are accepted 
				oper += expr[++position];

			Token t(oper);
			oper = "";
			if (empty())
				push_front(t);
			else
				push_back(t);
			++position;

		}
		else //Adds variables onto tokenlist
		{
			while (isalnum(expr[position]))
			{
				variable += expr[position];
				++position;
			}
			Token t(variable);
			variable = "";
			if (empty())
				push_front(t);
			else
				push_back(t);
		}

		while (expr[position] == ' ')   //get past any more spaces
			++position;
	}
}

//  output operation
//  Display all of the tokens in the list
ostream& operator<<( ostream &stream, TokenList &t )
{
    for (ListIterator iter = t.begin(); iter != t.end(); iter.advance())
    {
	stream << iter.token() << " ";
    }
    return stream;
}

//  Creates an iterator to refer to the beginning of the list
ListIterator TokenList::begin()
{
    return ListIterator( this, head );
}

//  Creates an iterator to refer after the end of the list
//  This list is simple:  it ends with NULL
ListIterator TokenList::end()
{
    return ListIterator( this, NULL );
}

//  Add a new element to the back of the list
//  Parameter:
//       t	(input Token)	the new item to add
void TokenList::push_back(Token t)
{
	ListElement* n = new ListElement;

	n->token = t;
	n->next = NULL;
	tail->next = n;
	tail = n;

}

//  Add a new element to the front of the list
//  Parameter:
//       t	(input Token)	the new item to add
void TokenList::push_front(Token t)
{
	ListElement* n = new ListElement;
	
	n->token = t;
	n->next = head;
	head = n;
	if (tail == NULL)
		tail = n;
}

//  Remove and return the element at the front of the list
Token TokenList::pop_front()
{
	ListElement* n = head;
	Token temp = head -> token;

	head = head->next;
	delete n;

	return temp;
}


This is a calculator created in C++ for CMPSC 122 Intermediate Programming at The Pennsylvania State University by Michael Berezanich.

Contact: michaelberezanich@gmail.com

This calculator has support for custom functions to be entered and stored in a map. Variables are supported as well and are stored in a binary tree. 

The .exe file may be found in the debug folder.

To define a variable while the program is running, when it asks, "Try one youself:", type "variable_name = value"
Where variable_name is the name of the variable you want to define or update and value is what you want it to be set equal to. This can be a number
value or another variable, in which case your new variable will equal whatever the other variable's value is.

To define a new function while the program is running, when it asks, "Try one youself:", type "deffn function_name(parameters) = function_definition"
Where function_name is what you want the name of your function to be, parameters what will be used in your function_definition (i.e. x,y) and 
function_definition is what you want to do with the parameters entered.
A sample of this is "deff sum(x,y) = x + y"
This will store the function for use in the rest of the program. To use your function simply call the name of your function and enter the parameters
to be used (i.e. entering sum(3,2) will return 5)
Variables are also supported as function parameters.

driver.cpp contains int main() so if you want to define variables/functions ahead of running the program you may add code in there as needed.


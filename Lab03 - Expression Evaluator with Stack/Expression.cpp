#include <iostream>
#include <sstream>
#include <string>
#include <stack>

using namespace std;

/**
* Function to determine the priority order of the arithmetic operations.
* According to the ascending order of priority, returns an integer.
*/
int opPriorityOrder(char c) {

    if (c == '*' || c == '/') {
        return 2;
    }
    else if (c == '+' || c == '-') {
        return 1;
    }
    else {
        return 0;
    }
}

/**
* Function which checks whether the given infix is faulty (valid) or not,
* by examining the parentheses of the infix expression.
*/
bool isFaultyExpression(string s) {

    stack<char> st;
    char temp;

    for (int i = 0; i < s.length(); i++) {
        
        if (s[i] == '(') {

            st.push(s[i]);
        }
        else if (s[i] == ')') {

            if (st.empty()) { 
                return false; 
            }
            
            temp = st.top();
            st.pop();

            if (s[i] == ')' && temp != '(') {
                return false;
            }
        }
    }

    if (st.empty()) { 
        return true; 
    }
    else {
        return false;
    }
}

///----------------------------------------------------------------------------------
/// Given an expression in infix-notation, converts it to a string in post-fix notation 
/// 
string Infix2Postfix(string& s) {
    
    stack<char> opStack;
    string result = "";

    // Check if the expression is valid, terminate the program if it's not.
    if (isFaultyExpression(s) == false)
    {
        cout << "Faulty expression: Paranthesis mismatch." << endl;
        return 0;
    }

    // Start iterating over the infix expression
    for (int i = 0; i < s.length(); i++)
    {
        // Check if you see an operator
        if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')
        {
            // Push it to the stack if the stack is empty
            if (opStack.empty()) {

                opStack.push(s[i]);
                result += ' ';
            } // Handled first element
            
            // If the stack already has items 
            else {
                
                // Compare priority of the operators 
                if (opPriorityOrder(s[i]) > opPriorityOrder(opStack.top())) {

                    opStack.push(s[i]);
                    result += ' ';
                }
                else if (opPriorityOrder(s[i]) == opPriorityOrder(opStack.top())) {

                    result = result + ' ' + opStack.top() + ' ';
                    opStack.pop();
                    opStack.push(s[i]);
                }
                else if (opPriorityOrder(s[i]) < opPriorityOrder(opStack.top())) {

                    while (!opStack.empty() && opPriorityOrder(s[i]) <= opPriorityOrder(opStack.top())) {
                        result = result + ' ' + opStack.top() + ' ';
                        opStack.pop();
                    }
                    
                    opStack.push(s[i]);
                } // Priority comparison ended
            }
        } // Operator handling is done

        // Start checking for paranthesis
        // Push it if it is opening paranthesis
        else if (s[i] == '(') {
            opStack.push(s[i]);
        }

        // Closing paranthesis encountered!
        // Start popping until you see the opening paranthesis
        else if (s[i] == ')') {

            while (opStack.top() != '(' && !opStack.empty()) {

                result = result + ' ' + opStack.top() + ' ';
                opStack.pop();
            }

            opStack.pop();
        }
        
        // Output the operands
        else {
            result += s[i];
        }
    } //Iteration over infix expression is done.

    // Start building the expression by unwrapping the stack
    // Carry on popping as long as there are items in the stack
    while (!opStack.empty()) {

        if (opStack.top() != '(') {
            result = result + ' ' + opStack.top() + ' ';
        }

        opStack.pop();
    }

    return result;
} // end-Infix2Postfix

///----------------------------------------------------------------------------------
/// Given a string in post-fix notation, evaluates it and returns the result
/// 
int EvaluatePostfixExpression(string& s) {
    
    stack<string> operand;
    string temp = "";

    for (int i = 0; i < s.size(); i++) {

        // Operand holders
        int x, y;

        // Handle Arithmetic Operations
        switch (s[i]) {
        case '*':
            // Assign x and y
            x = stoi(operand.top());
            operand.pop();

            y = stoi(operand.top());
            operand.pop();

            // Arithmetic operation
            operand.push(to_string(x * y));
            break;

        case '/':
            // Assign x and y
            x = stoi(operand.top());
            operand.pop();

            y = stoi(operand.top());
            operand.pop();

            // Handle division by zero
            if (x != 0) {
                operand.push(to_string(y / x));
            }
            else {
                // Notify the user about the error
                cout << "Error: Division by zero!" << endl;
                return 0;
            }
            break;

        case '+':
            // Assign x and y
            x = stoi(operand.top());
            operand.pop();

            y = stoi(operand.top());
            operand.pop();

            // Arithmetic operation
            operand.push(to_string(x + y));
            break;

        case '-':
            // Assign x and y
            x = stoi(operand.top());
            operand.pop();

            y = stoi(operand.top());
            operand.pop();

            // Arithmetic operation
            operand.push(to_string(y - x));
            break;

        default:
            // Handle spaces
            if (s[i] != ' ') {
                temp += s[i];
            }
            else if (s[i] == ' ' && temp.length() != 0) {
                operand.push(temp);
                temp = "";
            }
        }
    }

    return stoi(operand.top());
} // end-EvaluatePostfixExpression
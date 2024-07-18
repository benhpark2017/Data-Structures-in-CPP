/**
 * Solutions to Chapter 3, Section 6, Exercises 4 to 7 of Horowitz's
 * Fundamentals of Data Structures in C++.
 * 
 * The program now uses the Stack standard library instead of relying
 * on a user-defined header and implementation file for ADT Stack.
 */

#include <iostream>
#include <stack>
#include <sstream>
#include <vector>
#include <string>

// Function to check if the given character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

// Function to convert prefix to postfix
std::string prefixToPostfix(const std::string &prefix) {
    std::stack<std::string> stack;
    
    // Read the prefix expression from right to left
    for (int i = prefix.size() - 1; i >= 0; --i) {
        // If the character is an operator
        if (isOperator(prefix[i])) {
            // Pop two elements from stack
            std::string op1 = stack.top(); stack.pop();
            std::string op2 = stack.top(); stack.pop();
            
            // Concatenate the operands and operator in postfix order
            std::string temp = op1 + " " + op2 + " " + prefix[i];
            
            // Push the result back onto the stack
            stack.push(temp);
        } else if (prefix[i] != ' ') {
            // If the character is an operand (assuming single character operands for simplicity)
            stack.push(std::string(1, prefix[i]));
        }
    }
    
    // The final element of the stack is the postfix expression
    return stack.top();
}

// Function to convert postfix to prefix
std::string postfixToPrefix(const std::string &postfix) {
    std::stack<std::string> stack;

    // Read the postfix expression from left to right
    for (char c : postfix) {
        // If the character is an operand (assuming single character operands for simplicity)
        if (!isOperator(c) && c != ' ') {
            stack.push(std::string(1, c));
        } else if (isOperator(c)) {
            // Pop two elements from the stack
            std::string op2 = stack.top(); stack.pop();
            std::string op1 = stack.top(); stack.pop();

            // Concatenate the operator and operands in prefix order
            std::string temp = c + op1 + op2;

            // Push the result back onto the stack
            stack.push(temp);
        }
    }

    // The final element of the stack is the prefix expression
    return stack.top();
}

// Function to convert postfix to infix
std::string postfixToInfix(const std::string &postfix) {
    std::stack<std::string> stack;

    // Read the postfix expression from left to right
    for (char c : postfix) {
        // If the character is an operand (assuming single character operands for simplicity)
        if (!isOperator(c) && c != ' ') {
            stack.push(std::string(1, c));
        } else if (isOperator(c)) {
            // Pop two operands from the stack
            std::string op2 = stack.top(); stack.pop();
            std::string op1 = stack.top(); stack.pop();

            // Construct the infix expression
            std::string temp = "(" + op1 + " " + c + " " + op2 + ")";

            // Push the result back onto the stack
            stack.push(temp);
        }
    }

    // The final element of the stack is the infix expression
    return stack.top();
}

// Function to convert prefix to infix
std::string prefixToInfix(const std::string &prefix) {
    std::stack<std::string> stack;

    // Read the prefix expression from right to left
    for (int i = prefix.size() - 1; i >= 0; --i) {
        // If the character is an operand (assuming single character operands for simplicity)
        if (!isOperator(prefix[i]) && prefix[i] != ' ') {
            stack.push(std::string(1, prefix[i]));
        } else if (isOperator(prefix[i])) {
            // Pop two operands from the stack
            std::string op1 = stack.top(); stack.pop();
            std::string op2 = stack.top(); stack.pop();

            // Construct the infix expression
            std::string temp = "(" + op1 + " " + prefix[i] + " " + op2 + ")";

            // Push the result back onto the stack
            stack.push(temp);
        }
    }

    // The final element of the stack is the infix expression
    return stack.top();
}


int main() {
    // Test prefix to postfix conversion
    std::string prefixExpr1 = "*+AB-CD";
    std::string postfixExpr1 = prefixToPostfix(prefixExpr1);
    std::cout << "Prefix: " << prefixExpr1 << std::endl;
    std::cout << "Postfix: " << postfixExpr1 << std::endl;

    // Test postfix to prefix conversion
    std::string postfixExpr2 = "AB+CD-*";
    std::string prefixExpr2 = postfixToPrefix(postfixExpr2);
    std::cout << "Postfix: " << postfixExpr2 << std::endl;
    std::cout << "Prefix: " << prefixExpr2 << std::endl;

    // Test postfix to infix conversion
    std::string postfixExpr3 = "AB+CD-*";
    std::string infixExpr1 = postfixToInfix(postfixExpr3);
    std::cout << "Postfix: " << postfixExpr3 << std::endl;
    std::cout << "Infix: " << infixExpr1 << std::endl;

    // Test prefix to infix conversion
    std::string prefixExpr3 = "*+AB-CD";
    std::string infixExpr2 = prefixToInfix(prefixExpr3);
    std::cout << "Prefix: " << prefixExpr3 << std::endl;
    std::cout << "Infix: " << infixExpr2 << std::endl;

    return 0;
}

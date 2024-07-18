/**
 * Solutions to Chapter 3, Section 5, Exercise 1 of Horowitz's
 * Fundamentals of Data Structures in C++.
 */
 
/************************************main.cpp*************************************/
#include <iostream>
#include "ExpressionEvaluator.h"

int main() {
    
    char expression[MAX_EXPR_SIZE];
    std::cout << "Please enter an infix expression, e.g. 3*9/2+3\n";
    std::cout << "followed by Enter key: \n";
    std::cin.getline(expression, MAX_EXPR_SIZE);
    
    ExpressionEvaluator evaluator(expression);

    // Infix to Postfix conversion and evaluation
    evaluator.infixToPostfix();
    int postfixResult = evaluator.evaluatePostfix();
    std::cout << "Postfix evaluation result: " << postfixResult << std::endl;

    // Reset the evaluator with the original infix expression
    evaluator = ExpressionEvaluator(expression);

    // Infix to Prefix conversion and evaluation
    evaluator.infixToPrefix();
    int prefixResult = evaluator.evaluatePrefix();
    std::cout << "Prefix evaluation result: " << prefixResult << std::endl;

    return 0;
}



/************************************Stack.h**************************************/
#ifndef STACK_H
#define STACK_H

#include <iostream>

template<class KeyType>
class Stack {
private:
    KeyType* stack;
    int top;
    int capacity;

public:
    Stack(int size = 100); // Constructor prototype
    ~Stack();              // Destructor prototype
    void push(KeyType item);     // Push method prototype
    KeyType pop();               // Pop method prototype
    KeyType peek();              // Peek method prototype
    bool isEmpty();        // isEmpty method prototype
    bool isFull();         // isFull method prototype
};

#include "Stack.cpp"

#endif // STACK_H




/***********************************Stack.cpp*************************************/
#ifndef STACK_CPP
#define STACK_CPP

#include "Stack.h"

template<class KeyType>
Stack<KeyType>::Stack(int size) {
    stack = new KeyType[size];
    capacity = size;
    top = -1;
}

template<class KeyType>
Stack<KeyType>::~Stack() {
    delete[] stack;
}

template<class KeyType>
void Stack<KeyType>::push(KeyType item) {
    if (isFull()) {
        std::cerr << "The stack is full.\n";
        exit(1);
    }
    stack[++top] = item;
}

template<class KeyType>
KeyType Stack<KeyType>::pop() {
    if (isEmpty()) {
        std::cerr << "The stack is empty.\n";
        exit(1);
    }
    return stack[top--];
}

template<class KeyType>
KeyType Stack<KeyType>::peek() {
    if (isEmpty()) {
        std::cerr << "The stack is empty.\n";
        exit(1);
    }
    return stack[top];
}

template<class KeyType>
bool Stack<KeyType>::isEmpty() {
    return top == -1;
}

template<class KeyType>
bool Stack<KeyType>::isFull() {
    return top == capacity - 1;
}

#endif




/*******************************ExpressionEvaluator.h**********************************/
#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H

#include <iostream>
#include "Stack.h"

#define MAX_EXPR_SIZE 100

enum class Precedence {
    lparen, rparen, plus, minus,
    times, divide, mod, logical_and,
    logical_or, left_shift, right_shift,
    equal, not_equal, less, greater,
    less_equal, greater_equal, eos, operand,
};

class ExpressionEvaluator {
public:
    ExpressionEvaluator(const std::string& expression);
    void infixToPostfix();
    int evaluatePostfix();
    void infixToPrefix();
    int evaluatePrefix();

private:
    std::string expr;
    Precedence getToken(char* symbol, int* n);
    void setPostfixToken(Precedence token, std::string& str);
    void setPrefixToken(Precedence token, std::string& str);
};

#endif // EXPRESSION_EVALUATOR_H




/******************************ExpressionEvaluator.cpp*********************************/
#include "ExpressionEvaluator.h"
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <cctype> // for isdigit

const int isp[] = { 0,19,12,12,13,13,13,5,4,11,11,9,9,10,10,10,10,0 };
const int icp[] = { 20,19,12,12,13,13,13,5,4,11,11,9,9,10,10,10,10,0 };

ExpressionEvaluator::ExpressionEvaluator(const std::string& expression) : expr(expression) {}

Precedence ExpressionEvaluator::getToken(char *symbol, int *n) {
    *symbol = expr[(*n)++];
    switch (*symbol) {
        case '(': return Precedence::lparen;
        case ')': return Precedence::rparen;
        case '+': return Precedence::plus;
        case '-': return Precedence::minus;
        case '/': return Precedence::divide;
        case '*': return Precedence::times;
        case '%': return Precedence::mod;
        case '&': if (expr[*n] == '&') {
                      ++*(n); return Precedence::logical_and;
                  }
                  return Precedence::eos;
        case '|': if (expr[*n] == '|') {
                      ++*(n); return Precedence::logical_or;
                  }
                  return Precedence::eos;
        case '<': if (expr[*n] == '<') { 
                      ++(*n); return Precedence::left_shift;
                  } else if (expr[*n] == '=') {
                      ++(*n);
                      return Precedence::less_equal;
                  }
                  return Precedence::less;
        case '>': if (expr[*n] == '>') {
                      ++(*n);
                      return Precedence::right_shift;
                  } else if (expr[*n] == '=') {
                      ++(*n);
                      return Precedence::greater_equal;
                  }
                  return Precedence::greater;
        case '=': if (expr[*n] == '=') {
                      ++*(n); return Precedence::equal;
                  }
                  return Precedence::eos;
        case '!': if (expr[*n] =='=' ) {
                      ++*(n); return Precedence::not_equal;
                  }
                  return Precedence::eos;
        case '\0': return Precedence::eos;
        default : return Precedence::operand;
    }
}

void ExpressionEvaluator::setPostfixToken(Precedence token, std::string& str) {
    switch(token) {
        case Precedence::plus: str += '+'; return;
        case Precedence::minus: str += '-'; return;
        case Precedence::divide: str += '/'; return;
        case Precedence::times: str += '*'; return;
        case Precedence::mod: str += '%'; return;
        case Precedence::logical_and: str += "&&"; return;
        case Precedence::logical_or: str += "||"; return;
        case Precedence::left_shift: str += "<<"; return;
        case Precedence::right_shift: str += ">>"; return;
        case Precedence::equal: str += "=="; return;
        case Precedence::not_equal: str += "!="; return;
        case Precedence::less: str += '<'; return;
        case Precedence::greater: str += '>'; return;
        case Precedence::less_equal: str += "<="; return;
        case Precedence::greater_equal: str += ">="; return;
        case Precedence::eos: return;
        default: return;
    }
}

void ExpressionEvaluator::setPrefixToken(Precedence token, std::string& str) {
    switch(token) {
        case Precedence::plus: str.insert(str.begin(), '+'); return;
        case Precedence::minus: str.insert(str.begin(), '-'); return;
        case Precedence::divide: str.insert(str.begin(), '/'); return;
        case Precedence::times: str.insert(str.begin(), '*'); return;
        case Precedence::mod: str.insert(str.begin(), '%'); return;
        case Precedence::logical_and: str.insert(str.begin(), '&'); str.insert(str.begin(), '&'); return;
        case Precedence::logical_or: str.insert(str.begin(), '|'); str.insert(str.begin(), '|'); return;
        case Precedence::left_shift: str.insert(str.begin(), '<'); str.insert(str.begin(), '<'); return;
        case Precedence::right_shift: str.insert(str.begin(), '>'); str.insert(str.begin(), '>'); return;
        case Precedence::equal: str.insert(str.begin(), '='); str.insert(str.begin(), '='); return;
        case Precedence::not_equal: str.insert(str.begin(), '!'); str.insert(str.begin(), '='); return;
        case Precedence::less: str.insert(str.begin(), '<'); return;
        case Precedence::greater: str.insert(str.begin(), '>'); return;
        case Precedence::less_equal: str.insert(str.begin(), '<'); str.insert(str.begin(), '='); return;
        case Precedence::greater_equal: str.insert(str.begin(), '>'); str.insert(str.begin(), '='); return;
        case Precedence::eos: return;
        default: return;
    }
}

void ExpressionEvaluator::infixToPostfix() {
    char symbol;
    std::string expr_post;
    Precedence token;
    int n = 0;

    Stack<Precedence> stack(MAX_EXPR_SIZE);
    stack.push(Precedence::eos);

    for (token = getToken(&symbol, &n); token != Precedence::eos; token = getToken(&symbol, &n)) {
        if (token == Precedence::operand) {
            expr_post += symbol;
        } else if (token == Precedence::rparen) {
            while (stack.peek() != Precedence::lparen)
                setPostfixToken(stack.pop(), expr_post);
            stack.pop();
        } else {
            while (isp[static_cast<int>(stack.peek())] >= icp[static_cast<int>(token)])
                setPostfixToken(stack.pop(), expr_post);
            stack.push(token);
        }
    }

    while ((token = stack.pop()) != Precedence::eos)
        setPostfixToken(token, expr_post);

    std::cout << "Postfix expression: " << expr_post << std::endl;
    expr = expr_post;
}

int ExpressionEvaluator::evaluatePostfix() {
    Precedence token;
    char symbol;
    int op1, op2;
    int n = 0;

    Stack<int> stack(MAX_EXPR_SIZE);

    token = getToken(&symbol, &n);
    while (token != Precedence::eos) {
        if (token == Precedence::operand) {
            stack.push(symbol - '0');
        } else {
            op2 = stack.pop();
            op1 = stack.pop();
            switch (token) {
                case Precedence::plus: stack.push(op1 + op2); break;
                case Precedence::minus: stack.push(op1 - op2); break;
                case Precedence::times: stack.push(op1 * op2); break;
                case Precedence::divide: stack.push(op1 / op2); break;
                case Precedence::mod: stack.push(op1 % op2); break;
                case Precedence::logical_and: stack.push(op1 && op2); break;
                case Precedence::logical_or: stack.push(op1 || op2); break;
                case Precedence::left_shift: stack.push(op1 << op2); break;
                case Precedence::right_shift: stack.push(op1 >> op2); break;
                case Precedence::equal: stack.push(op1 == op2); break;
                case Precedence::not_equal: stack.push(op1 != op2); break;
                case Precedence::less: stack.push(op1 < op2); break;
                case Precedence::greater: stack.push(op1 > op2); break;
                case Precedence::less_equal: stack.push(op1 <= op2); break;
                case Precedence::greater_equal: stack.push(op1 >= op2); break;
                default: break;
            }
        }
        token = getToken(&symbol, &n);
    }
    return stack.pop();
}

void ExpressionEvaluator::infixToPrefix() {
    char symbol;
    std::string expr_prefix;
    std::string temp; // Temporary string to store the reversed expression
    Precedence token;
    int n = 0;

    // Reverse the input expression and change '(' to ')' and vice versa
    std::reverse(expr.begin(), expr.end());
    for (char& ch : expr) {
        if (ch == '(') {
            ch = ')';
        } else if (ch == ')') {
            ch = '(';
        }
    }

    Stack<Precedence> stack(MAX_EXPR_SIZE);
    stack.push(Precedence::eos);

    // Process the reversed infix expression
    for (token = getToken(&symbol, &n); token != Precedence::eos; token = getToken(&symbol, &n)) {
        if (token == Precedence::operand) {
            expr_prefix.insert(expr_prefix.begin(), symbol);
        } else if (token == Precedence::rparen) {
            while (stack.peek() != Precedence::lparen)
                setPrefixToken(stack.pop(), expr_prefix);
            stack.pop();
        } else {
            while (isp[static_cast<int>(stack.peek())] > icp[static_cast<int>(token)])
                setPrefixToken(stack.pop(), expr_prefix);
            stack.push(token);
        }
    }

    while ((token = stack.pop()) != Precedence::eos)
        setPrefixToken(token, expr_prefix);

    std::cout << "Prefix expression: " << expr_prefix << std::endl;
    expr = expr_prefix;
}

int ExpressionEvaluator::evaluatePrefix() {
    Stack<int> stack(MAX_EXPR_SIZE);

    // Traverse the prefix expression from right to left
    for (auto it = expr.rbegin(); it != expr.rend(); ++it) {
        char symbol = *it;

        if (std::isdigit(symbol)) {
            stack.push(symbol - '0');
        } else {
            if (stack.isEmpty()) {
                std::cerr << "Error: Invalid prefix expression" << std::endl;
                return 0;
            }
            int op1 = stack.pop();

            if (stack.isEmpty()) {
                std::cerr << "Error: Invalid prefix expression" << std::endl;
                return 0;
            }
            int op2 = stack.pop();

            int result;
            switch (symbol) {
                case '+': result = op1 + op2; break;
                case '-': result = op1 - op2; break;
                case '*': result = op1 * op2; break;
                case '/': 
                    if (op2 == 0) {
                        std::cerr << "Error: Division by zero" << std::endl;
                        return 0;
                    }
                    result = op1 / op2; 
                    break;
                default: 
                    std::cerr << "Error: Unknown operator " << symbol << std::endl;
                    return 0;
            }
            stack.push(result);
        }
    }

    if (stack.isEmpty()) {
        std::cerr << "Error: Invalid prefix expression" << std::endl;
        return 0;
    }
    int final_result = stack.pop();

    if (!stack.isEmpty()) {
        std::cerr << "Error: Invalid prefix expression (too many operands)" << std::endl;
        return 0;
    }

    return final_result;
}

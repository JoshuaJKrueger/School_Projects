'''
    Joshua J. Krueger
    CS 2420-002 Project 4
    This program converts infix expressions to postfix expressions
     using a stack ADT.
'''

import operator
from typing import Final
from stack import Stack

def in2post(expr:str):
    '''
        Takes infix expression and returns it as postfix
    '''
    if not isinstance(expr, str):
        raise ValueError

    expr = expr.replace(' ', '')
    stack = Stack()
    postfix = ''

    precedence:Final = {'+': 1, '-': 1, '*': 2, '/': 2}

    for char in expr:
        if char == '(':
            stack.push(char)
        elif char.isnumeric():
            postfix += char
        elif char in '+-*/':
            while (stack.size() > 0 and
                   stack.top() != '(' and
                   precedence[stack.top()] >= precedence[char]):
                postfix += stack.pop()
            stack.push(char)
        else:
            if char != ')':
                raise SyntaxError

            postfix += stack.pop()

            if stack.size() == 0:
                raise SyntaxError

            while stack.top() != '(':
                postfix += stack.pop()

            if stack.pop() != '(':
                raise SyntaxError

    while stack.size() > 0:
        postfix += stack.pop()

    return postfix


def eval_postfix(expr:str):
    '''
        Receives postfix string returns the result
    '''
    if not isinstance(expr, str):
        raise ValueError

    expr = expr.replace(' ', '')
    stack = Stack()
    second_oper = None
    first_oper = None

    for char in expr:
        if char.isnumeric():
            stack.push(char)
            continue
        if char in '+-*/':
            if stack.size() < 2:
                raise SyntaxError

            second_oper = float(stack.pop())
            first_oper = float(stack.pop())

            push_result = lambda oper: stack.push(oper(first_oper, second_oper))

            if char == '+':
                push_result(operator.add)
            elif char == '-':
                push_result(operator.sub)
            elif char == '*':
                push_result(operator.mul)
            elif char == '/':
                push_result(operator.truediv)
        else:
            raise SyntaxError

    return float(stack.pop())


def main():
    '''
        Entrypoint of program
    '''
    data_filename:Final = 'data.txt'

    data = open(data_filename, 'r').readlines()

    for line in data:
        infix = line.replace(' ', '').strip('\n')

        print(f'infix: {infix}')

        postfix = in2post(infix)

        print(f'postfix: {postfix}')
        print(f'answer: {eval_postfix(postfix)}\n')


if __name__ == '__main__':
    main()

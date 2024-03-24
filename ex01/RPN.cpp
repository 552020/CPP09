#include "RPN.hpp"

RPN::RPN(const std::string &input) : _input(input)
{
}

RPN::~RPN()
{
}

bool RPN::isOperator(char c) const
{
	return c == '+' || c == '-' || c == '*' || c == '/';
}

bool RPN::isValidCharacter(char c) const
{
	return isdigit(c) || isOperator(c) || c == ' ';
}

bool RPN::isValid() const
{
	int operands = 0;
	int operators = 0;
	bool expressionIsBalanced = false;
	for (std::size_t i = 0; i < _input.length(); ++i)
	{
		if (isdigit(_input[i]))
		{
			if ((i - 1 < _input.length()) && isdigit(_input[i + 1]))
				return false;
			operands++;
		}
		else if (isOperator(_input[i]))
			operators++;
		else if (_input[i] != ' ')
			return false;
	}
	expressionIsBalanced = operands == operators + 1;
	return expressionIsBalanced;
}

int RPN::calculate()
{
	if (!isValid())
	{
		std::cout << "Invalid expression." << std::endl;
		throw std::runtime_error("Error");
	}

	std::stack<int> operands;
	for (std::size_t i = 0; i < _input.length(); ++i)
	{
		if (isdigit(_input[i]))
		{
			// Convert char to int and push
			operands.push(_input[i] - '0');
		}
		else if (isOperator(_input[i]))
		{
			int firstOperand = operands.top();
			operands.pop();
			int secondOperand = operands.top();
			operands.pop();
			switch (_input[i])
			{
			case '+':
				operands.push(firstOperand + secondOperand);
				break;
			case '-':
				operands.push(secondOperand - firstOperand);
				break;
			case '*':
				operands.push(firstOperand * secondOperand);
				break;
			case '/':
				if (firstOperand == 0)
				{
					throw std::runtime_error("Error: Division by zero.");
				}
				operands.push(secondOperand / firstOperand);
				break;
			}
		}
	}
	return operands.top();
}
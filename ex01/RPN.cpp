#include "RPN.hpp"
#include <cctype>

std::string trim(const std::string &input)
{
	std::string trimmedInput = input;

	// Remove leading spaces
	std::string::iterator it = trimmedInput.begin();
	while (it != trimmedInput.end() && std::isspace(*it))
		++it;
	trimmedInput.erase(trimmedInput.begin(), it);

	// Remove trailing spaces
	it = trimmedInput.end();
	while (it != trimmedInput.begin() && std::isspace(*(it - 1)))
		--it;
	trimmedInput.erase(it, trimmedInput.end());

	return trimmedInput;
}

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
	std::string trimmedInput = trim(_input);

	if (trimmedInput.length() < 2)
	{
		std::cout << "Error: The expression is too short to be valid." << std::endl;
		return false;
	}

	if (!std::isdigit(trimmedInput[0]) || !std::isdigit(trimmedInput[2]))
	{
		std::cout << "Error: The first two characters must be digits." << std::endl;
		return false;
	}

	int operands = 0;
	int operators = 0;
	bool expressionIsBalanced = false;

	// std::cout << "trimmedInput: " << trimmedInput << std::endl;

	for (std::size_t i = 0; i < _input.length(); ++i)
	{
		if (isdigit(trimmedInput[i]))
		{
			// We check if the next character is a digit, cause only ond digit is allowed
			// std::cout << trimmedInput[i] << std::endl;
			if (i + 1 < trimmedInput.length())
			{
				if (std::isdigit(trimmedInput[i + 1]))
				{
					std::cout << "Error: Multi-digit numbers are not allowed." << std::endl;
					return false;
				}
				if (trimmedInput[i + 1] != ' ')
				{
					std::cout << "Error: There must be a space after each operand." << std::endl;
					return false;
				}
			}
			else if (i + 1 == trimmedInput.length())
			{
				std::cout << "Error: The last character must be an operator." << std::endl;
			}
			operands++;
		}
		else if (isOperator(trimmedInput[i]))
		{
			operators++;
			if (i + 1 < trimmedInput.length() && trimmedInput[i + 1] != ' ')
			{
				std::cout << "Error: There must be a space after every operator." << std::endl;
				return false;
			}
		}
		else if ((i + 1 < trimmedInput.length()) && trimmedInput[i] != ' ')
		{
			// std::cout << i << std::endl;
			std::cout << trimmedInput[i] << std::endl;
			std::cout << "Error: Invalid character found in the expression." << std::endl;
			return false; // Invalid character
		}
	}
	expressionIsBalanced = operands == operators + 1;
	if (!expressionIsBalanced)
	{
		std::cout << "Error: The expression is not balanced." << std::endl;
		return false;
	}
	return true;
}

int RPN::calculate()
{
	if (!isValid())
	{
		throw std::runtime_error("Error: Invalid Expression");
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
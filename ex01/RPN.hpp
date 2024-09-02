#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <string>
#include <stdexcept>

std::string trim(const std::string &input);

class RPN
{
  public:
	RPN(const std::string &input);
	~RPN();
	int calculate();
	bool isValid() const;

  private:
	std::string _input;
	// std::stack<int> _stack;

	RPN(const RPN &other);
	RPN &operator=(const RPN &other);

	bool isOperator(char c) const;
	bool isValidCharacter(char c) const;
};

#endif
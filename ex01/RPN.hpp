#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <string>
#include <stdexcept>

class RPN
{
  public:
	// Default constructor
	RPN(const std::string &input);
	// Copy constructor
	// Copy assignment operator
	// Destructor
	~RPN();
	int calculate();
	bool isValid() const;

  private:
	std::string _input;
	std::stack<int> _stack;

	RPN(const RPN &other);
	RPN &operator=(const RPN &other);

	bool isOperator(char c) const;
	bool isValidCharacter(char c) const;
};

#endif
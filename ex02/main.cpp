#include <string>
#include <iostream>
#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
	std::string input;
	if (argc < 2)
	{
		std::cout << "Usage " << argv[0] << " <some numbers>. Only positive numbers. Sign is also not tolerated!"
				  << std::endl;
		return 1;
	}
	else if (argc == 2)
	{
		input = argv[1];
	}
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			// return if argv[i] is not just a number
			std::string tmp(argv[i]);
			if (tmp.find_first_not_of("0123456789") != std::string::npos)
			{
				std::cout << "Invalid input. Only positive numbers are allowed, with no signes. Mixed inputs like '\"1 "
							 "2\" 2' is also not tolerated!"
						  << std::endl;
				return 1;
			}
			if (i > 1)
				input += " ";
			input += argv[i];
		}
	}

	PmergeMe obj;
	obj.sortList(input);
	obj.sortVec(input);
	return 0;
}

#include <iostream>
#include <stack>
#include <vector>
#include <list>

// g++ -std=c++11 stack.cpp -o stack

template<typename T>
void checkUnderlyingContainer(const T& stack) {
    typedef typename T::container_type ContainerType;
    std::cout << "Underlying container type: " << typeid(ContainerType).name() << std::endl;
}

int main() {
    // Create a stack of integers
    std::stack<int> myStack;

    // Push elements onto the stack
    myStack.push(10);
    myStack.push(20);
    myStack.push(30);

    // Access the top element
    std::cout << "Top element: " << myStack.top() << std::endl;

    // Pop the top element
    myStack.pop();

    // Access the new top element
    std::cout << "Top element after pop: " << myStack.top() << std::endl;

    // Check if the stack is empty
    if (!myStack.empty()) {
        std::cout << "Stack is not empty." << std::endl;
    }

    // Print stack size
    std::cout << "Stack size: " << myStack.size() << std::endl;

	  // Stack using std::deque (default)
    std::stack<int> defaultStack;
    defaultStack.push(1);
    defaultStack.push(2);
    std::cout << "Top element in default stack (deque): " << defaultStack.top() << std::endl;

    // Stack using std::vector
    std::stack<int, std::vector<int> > vectorStack;
    vectorStack.push(3);
    vectorStack.push(4);
    std::cout << "Top element in vector-based stack: " << vectorStack.top() << std::endl;

    // Stack using std::list
    std::stack<int, std::list<int> > listStack;
    listStack.push(5);
    listStack.push(6);
    std::cout << "Top element in list-based stack: " << listStack.top() << std::endl;

	checkUnderlyingContainer(defaultStack);
	checkUnderlyingContainer(vectorStack);
	checkUnderlyingContainer(listStack);


    return 0;

    return 0;
}

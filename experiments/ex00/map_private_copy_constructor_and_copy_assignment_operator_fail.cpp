#include <iostream>
#include <map>
#include <string>

// g++ -std=c++98 -o map_private_copy_constructor_and_copy_assignment_operator_fail map_private_copy_constructor_and_copy_assignment_operator_fail.cpp

class Key {
public:
    Key(const std::string& value) : value(value) {}

    // Custom comparison operator
    bool operator<(const Key& other) const {
        return value < other.value;
    }

private:
    std::string value;

    // Declare copy constructor and copy assignment operator as private 
	// Only the copy constructor needs to be available to the map
    // Key(const Key& other);
    Key& operator=(const Key& other);
};

int main() {
    std::map<Key, int> myMap;

    Key k1("key1");
    Key k2("key2");

    myMap[k1] = 1;
    myMap[k2] = 2;

    return 0;
}

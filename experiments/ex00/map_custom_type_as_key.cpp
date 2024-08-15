#include <iostream>
#include <map>
#include <string>

// g++ -std=c++98 -o map_custom_type_as_key map_custom_type_as_key.cpp

struct Person {
    std::string firstName;
    std::string lastName;

    // Custom comparison operator
    bool operator<(const Person& other) const {
        if (lastName == other.lastName) {
            return firstName < other.firstName;
        }
        return lastName < other.lastName;
    }
};

int main() {
    std::map<Person, int> personMap;

    Person p1 = {"John", "Doe"};
    Person p2 = {"Jane", "Smith"};
    Person p3 = {"John", "Smith"};

    personMap[p1] = 1;
    personMap[p2] = 2;
    personMap[p3] = 3;

    // Iterate over the map and print the contents
    for (std::map<Person, int>::iterator it = personMap.begin(); it != personMap.end(); ++it) {
        std::cout << it->first.firstName << " " << it->first.lastName << ": " << it->second << std::endl;
    }

    return 0;
}

#include <iostream>
#include <map>
#include <string>

// g++ -std=c++98 -o map map.cpp


int main() {
    // Create a map that associates strings (keys) with integers (values)
    std::map<std::string, int> myMap;

    // Insert key-value pairs into the map
    myMap["apple"] = 5;
    myMap["banana"] = 3;
    myMap["orange"] = 7;

    // Iterate over the map and print the contents
    std::map<std::string, int>::iterator it;
    for (it = myMap.begin(); it != myMap.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

	    // Add a new element
    myMap["grape"] = 10;
	// Print the map 
	std::cout << "After adding a new element:" << std::endl;
	for (it = myMap.begin(); it != myMap.end(); ++it) {
		std::cout << it->first << ": " << it->second << std::endl;
	}

	// Find an element in the map
	it = myMap.find("banana");
	// Print the found element
	std::cout << "Found element: " << it->first << ": " << it->second << std::endl;

	// Erase an element from the map
	myMap.erase("banana");
	// Print the map after erasing an element
	std::cout << "After erasing an element:" << std::endl;
	for (it = myMap.begin(); it != myMap.end(); ++it) {
		std::cout << it->first << ": " << it->second << std::endl;
	}

	// Check if a key exists in the map
	if (myMap.count("banana") > 0) {
		std::cout << "Key 'banana' exists in the map" << std::endl;
	} else {
		std::cout << "Key 'banana' does not exist in the map" << std::endl;
	}

	// Change the value associated with a key
	myMap["apple"] = 10;
	// Print the map after changing the value
	std::cout << "After changing the value associated with 'apple':" << std::endl;
	for (it = myMap.begin(); it != myMap.end(); ++it) {
		std::cout << it->first << ": " << it->second << std::endl;
	}

	  // Iterate over the map and print the contents and addresses
    for (it = myMap.begin(); it != myMap.end(); ++it) {
        std::cout << it->first << ": " << it->second 
                  << " (Key Address: " << &(it->first) 
                  << ", Value Address: " << &(it->second) << ")" 
                  << std::endl;
    }

    return 0;
}

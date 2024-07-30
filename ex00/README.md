# ex00 - Bitcoing Exchange

## TODOs

1. **Setup and Initialization**

   - [x] Create a new directory named `ex00/`.
   - [x] Create the following files:
     - `Makefile`
     - `main.cpp`
     - `BitcoinExchange.cpp`
     - `BitcoinExchange.hpp`

2. **Makefile Configuration**

   - [x] Ensure the `Makefile` has the following rules:
     - `$(NAME)`
     - `all`
     - `clean`
     - `fclean`
     - `re`
   - [x] Compile the program with `c++` and the flags `-Wall -Wextra -Werror -std=c++98`.
   - [x] Set the output program name to `btc`.

3. **Database Parsing and Validation**

We will check the presence and integrity of the database file before we check and parse the command-line argument.

- **CSV Database:**

  - Create a function `parseDatabase` to read and parse the CSV database.
  - Create a function `checkDatabase` to validate the structure of the CSV file.
  - Ensure the database has the correct format: `date,exchange_rate`.
  - Store the data in a `std::map<std::string, float>` for efficient date-based lookups.

- **Input File:**
  - Create a function `parseInputFile` to read and parse the input file.
  - Ensure each line in the input file follows the format: `date | value`.
  - Implement a function `checkDate` to validate the date format (`YYYY-MM-DD`).
  - Implement a function `checkValue` to validate the value (float or positive integer between 0 and 1000).

4. **Command-Line Argument Parsing**

   - In `main.cpp`, implement argument parsing to ensure exactly one argument is passed (the input file).
   - Implement `checkFile` function to verify the existence and readability of the file.

5. **Processing and Calculation**

   - For each entry in the input file:
     - Validate the date and value.
     - Use the date to lookup the closest date in the database (using `std::map`).
     - Calculate the result by multiplying the value by the exchange rate for the closest date.
     - Handle errors appropriately, displaying error messages for invalid entries.

6. **Output Results**

   - Output the results in the specified format:
     - `date => value = calculated_value`
   - Ensure all output messages end with a new-line character.

7. **Error Handling**

   - Handle file not found errors.
   - Handle invalid date and value errors.
   - Display appropriate error messages for each type of error.

8. **Memory Management**
   - Ensure no memory leaks occur.
   - Use RAII principles for resource management.

### Functions Overview

- **Main Functions:**

  - `int main(int argc, char* argv[])`
  - `void checkFile(const std::string& fileName)`
  - `void parseDatabase(const std::string& dbFileName, std::map<std::string, float>& exchangeRates)`
  - `void checkDatabase(const std::string& dbFileName)`
  - `void parseInputFile(const std::string& inputFileName, const std::map<std::string, float>& exchangeRates)`
  - `bool checkDate(const std::string& date)`
  - `bool checkValue(const std::string& value)`

- **Helper Functions:**
  - `std::string trim(const std::string& str)`
  - `float stringToFloat(const std::string& str)`

# Concepts, objects

`std::ifstream` is a class from the C++ Standard Library used to read data from files. It is part of the <fstream> header. An ifstream object is used to open a file, check if it can be read, and read data from the file. If the file cannot be opened, operations on the ifstream object will fail.

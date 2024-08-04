#!/bin/bash

# Path to your RPN program executable
RPN_EXECUTABLE="./rpn"

# Define an array of test cases
# Each test case is a string containing: "expression expected_output"
test_cases=(
    "1 2 + 3"       # Expected output: 3
    "1 2 + 3 *"     # Expected output: 9
    "4 2 / 2"       # Expected output: 2
    "3 5 8 * 7 + *" # Expected output: 141
    "5 1 2 + 4 * + 3 -" # Expected output: 14
    "8 9 * 9 - 9 - 9 - 4 - 1 +" # Expected output: 42
    "7 7 * 7 -"     # Expected output: 42
    "1 2 * 2 / 2 * 2 4 - +" # Expected output: 0
    "(1 + 1)"       # Expected output: Error (Invalid expression)
    "1 3 7 - +"     # Expected output: -3
    "3 4 5 * + 2 -" # Expected output: 21
)

# Function to run a single test case
run_test() {
    expression="$1"
    expected_output="$2"

    # Run the RPN program with the expression
    actual_output=$($RPN_EXECUTABLE "$expression")

    # Check if the actual output matches the expected output
    if [[ "$actual_output" == "$expected_output" ]]; then
        echo "Test passed for expression: '$expression'"
    else
        echo "Test FAILED for expression: '$expression'"
        echo "Expected: '$expected_output', but got: '$actual_output'"
    fi
}

# Iterate over each test case and run the test
for test_case in "${test_cases[@]}"; do
    # Split the test case into expression and expected output
    expression=$(echo "$test_case" | awk '{print $1}')
    expected_output=$(echo "$test_case" | awk '{print $2}')

    # Run the test case
    run_test "$expression" "$expected_output"
done

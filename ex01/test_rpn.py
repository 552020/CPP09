import subprocess

# Define a list of tuples, where each tuple is an (expression, expected_output)
test_cases = [
    (
        "1 2 + 3",
        "Error: The last character must be an operator.",
    ),
    ("1 2 +", "Error: The expression is not balanced."),
    ("4 2 /", "2"),
    ("3 5 8 * 7 + *", "141"),
    ("5 1 2 + 4 * + 3 -", "14"),
    ("8 9 * 9 - 9 - 9 - 4 - 1 +", "42"),
    ("7 7 * 7 -", "42"),
    ("1 2 * 2 / 2 * 2 4 - +", "0"),
    ("(1 + 1)", "Error: The first two characters must be digits."),
    ("1 3 7 - +", "-3"),
    ("3 4 5 * + 2 -", "21"),
]

# Path to your RPN program executable
RPN_EXECUTABLE = "./rpn"


def run_test(expression, expected_output):
    # Run the RPN program with the expression
    result = subprocess.run(
        [RPN_EXECUTABLE, expression], capture_output=True, text=True
    )
    actual_output = result.stdout.strip()

    # Check if the actual output matches the expected output
    if actual_output == expected_output:
        print(f"Test passed for expression: '{expression}'")
    else:
        print(f"Test FAILED for expression: '{expression}'")
        print(f"Expected: '{expected_output}', but got: '{actual_output}'")


# Iterate over each test case and run the test
for expression, expected_output in test_cases:
    run_test(expression, expected_output)

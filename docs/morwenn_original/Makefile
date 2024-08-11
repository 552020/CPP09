# Compiler and Flags
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g -fsanitize=address -fsanitize=undefined

# Target executable name
TARGET = merge_insertion_sort_test

# Source and Object Files
SRCS = morwenn.cpp
OBJS = $(SRCS:.cpp=.o)

# Build Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

re: clean all

.PHONY: all clean re

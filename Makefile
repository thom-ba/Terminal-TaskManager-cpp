# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./

# Source files
SRC = main.cpp task.cpp term_utils.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Executable name
TARGET = task_manager

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)

# Clean up build files and remove executable
distclean: clean
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean distclean


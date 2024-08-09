# Define the compiler and compilation flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Define the target executable
TARGET = task_manager

# Define the source files
SRCS = main.cpp task.cpp term_utils.cpp

# Define the object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up generated files
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean


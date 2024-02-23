# Compiler settings - Can be customized.
CXX = g++
CXXFLAGS = -Wall -std=c++20
TARGET = splitl1cache
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compiling
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: clean

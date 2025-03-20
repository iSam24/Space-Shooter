# Compiler
CXX = g++

# Directories
SRC_DIR = src
SFML_DIR = C:/Users/SamWh/Development_Software/SFML-2.6.2

# Compiler & Linker Flags
CXXFLAGS = -I"$(SFML_DIR)/include" -Wall -std=c++17
LDFLAGS = -L"$(SFML_DIR)/lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# Output executable
TARGET = game.exe

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

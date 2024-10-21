CC = g++
CFLAGS = -std=c++17
INCLUDE = -I./include

# Source files
SOURCES = src/main.cpp src/hash_map.cpp src/trie.cpp src/utils.cpp src/Vector.cpp
# Object files
OBJECTS = $(SOURCES:.cpp=.o)
# Executable name
EXECUTABLE = MiniSearchEngine

# Rule for building the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

# Rule for compiling object files
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

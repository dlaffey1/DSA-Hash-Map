CC = g++
CFLAGS = -std=c++17

# Source files
SOURCES = src/main.cpp src/hash_map.cpp src/trie.cpp src/utils.cpp
# Object files
OBJECTS = $(SOURCES:.cpp=.o)
# Executable name
EXECUTABLE = MiniSearchEngine

# Rule for building the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

# Rule for compiling object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

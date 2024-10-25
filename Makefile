CC = g++
CFLAGS = -std=c++17
INCLUDE = -I./include

# Source files
SOURCES = src/main.cpp src/hash_map.cpp src/trie.cpp src/utils.cpp src/Vector.cpp src/parser.cpp src/Serializer.cpp
TEST_SOURCES = tests/extensive_tests.cpp src/hash_map.cpp src/trie.cpp src/utils.cpp src/Vector.cpp src/Serializer.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = MiniSearchEngine
TEST_EXECUTABLE = MiniSearchEngineTests

# Rule for building the main executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

# Rule for building the test executable
tests: $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) -o $(TEST_EXECUTABLE)

# Rule for compiling object files
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(TEST_OBJECTS) $(TEST_EXECUTABLE)

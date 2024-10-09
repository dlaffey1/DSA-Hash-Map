CC = gcc
CFLAGS = -Wall -g
SOURCES = src/main.c src/hash_map.c src/trie.c src/utils.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = MiniSearchEngine

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# DSA-Hash-Map
Project Structure
trie_tokenizer.c: The main C file containing the implementation of the trie, file reading, and word insertion functions.
Books/: Directory containing the example text files (example1.txt, example2.txt).
How the Trie Works:
Each node in the trie represents a character from a word.
When a word is inserted, it is broken down into its characters, and each character is placed in the appropriate node in the trie.
Once the end of the word is reached, the document (file path) is stored at that node, marking the word as complete.
The program reads from example1.txt and example2.txt, inserts the words into the trie, and stores the document path, but does not display the trie or its contents.
Example of Trie Structure:
If the words "test" and "this" are inserted, the trie might look like this:

less
Copy code
Root
├── t
│   ├── e
│   │   └── s
│   │       └── t (End, document: example.txt)
│   └── h
│       └── i
│           └── s (End, document: example.txt)
File Structure
bash
Copy code
├── Books
│   ├── example1.txt   # Example input file 1
│   ├── example2.txt   # Example input file 2
├── trie_tokenizer.c    # C program for the trie implementation
├── README.md           # This README file
How to Compile and Run the Program
Prerequisites:
GCC Compiler: Ensure GCC is installed on your system to compile the C program.
Compilation:
Open a terminal and navigate to the project directory.
Run the following command to compile the trie_tokenizer.c file:
bash
Copy code
gcc -o trie_tokenizer trie_tokenizer.c
Running the Program:
After compiling, run the program with:

bash
Copy code
./trie_tokenizer
The program will read the files example1.txt and example2.txt from the Books/ directory and insert the words into the trie, associating each word with the document it came from.
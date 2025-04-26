CC = gcc
COMPILER_FLAGS = -Wall -Wfatal-errors
LANG_STD = -std=c99
SOURCE_FILES = src/*.c
OUTPUT = renderer

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(SOURCE_FILES) -o $(OUTPUT)

run:
	./$(OUTPUT)

clean:
	rm $(OUTPUT)

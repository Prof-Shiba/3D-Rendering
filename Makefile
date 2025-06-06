CC = gcc
COMPILER_FLAGS = -Wall -Wfatal-errors
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lm
LANG_STD = -std=c99
SOURCE_FILES = src/*.c math/*.c
OUTPUT = renderer

build:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(LANG_STD) $(SOURCE_FILES) -o $(OUTPUT)

run:
	./$(OUTPUT)

clean:
	rm $(OUTPUT)

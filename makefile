CC = gcc

CFLAGS = -Wall -Wextra -g -c
LDFLAGS = -lncurses

SRC_DIR = src
SOURCES = $(SRC_DIR)/main.c

OBJECT_DIR = obj
OBJECTS = $(OBJECT_DIR)/main.o

OUTPUT_DIR = build
OUTPUT_FILE = out

all: $(OBJECTS)
	$(CC) -g $(OBJECTS) -o $(OUTPUT_FILE) $(LDFLAGS)

$(OBJECT_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) -o $(OBJECT_DIR)/main.o $(CFLAGS) $(SRC_DIR)/main.c

clean:
	rm -f $(OBJECTS) $(OUTPUT_DIR)/$(OUTPUT_FILE)

run: $(OUTPUT_DIR)/$(OUTPUT_FILE)
	./$(OUTPUT_DIR)/$(OUTPUT_FILE)


CC = gcc

CFLAGS = -Wall -Wextra -g -c
LDFLAGS = -lncurses

SRC_DIR = src
SOURCES = $(SRC_DIR)/main.c

OBJECT_DIR = obj
OBJECTS = $(OBJECT_DIR)/main.o

OUTPUT_DIR = build
OUTPUT_FILE = out

all: $(OBJECTS) | $(OUTPUT_DIR)
	$(CC) -g $(OBJECTS) -o $(OUTPUT_DIR)/$(OUTPUT_FILE) $(LDFLAGS)

$(OBJECT_DIR)/main.o: $(SRC_DIR)/main.c | $(OBJECT_DIR)
	$(CC) -o $(OBJECT_DIR)/main.o $(CFLAGS) $(SRC_DIR)/main.c

$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -f $(OBJECTS) $(OUTPUT_DIR)/$(OUTPUT_FILE)

run: $(OUTPUT_DIR)/$(OUTPUT_FILE)
	./$(OUTPUT_DIR)/$(OUTPUT_FILE)


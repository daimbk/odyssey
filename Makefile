CC = gcc
CFLAGS = -Wall -g
INCLUDES = -Iinclude

SRC_DIR = src
BUILD_DIR = build

# List of source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# List of object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Your executable name
EXECUTABLE = shell

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(EXECUTABLE) $(OBJ_FILES)

CC = gcc
CFLAGS = -Wall -g
INCLUDES = -Iinclude
LIBS = -lreadline

INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build

# source files
INCLUDE_FILES = $(wildcard $(INCLUDE_DIR)/*.h)
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

EXECUTABLE = odyssey

# dependencies
INSTALL_DEPS = build-essential zip unzip libreadline-dev clang-format

.PHONY: all install clean format

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

install:
	# Install necessary tools using the package manager
	sudo apt-get install -y $(INSTALL_DEPS)

format:
	clang-format -i $(INCLUDE_FILES) $(SRC_FILES)

clean:
	rm -f $(EXECUTABLE) $(OBJ_FILES)

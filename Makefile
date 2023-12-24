CC = gcc
CFLAGS = -Wall -g
INCLUDES = -Iinclude
LIBS = -lreadline

SRC_DIR = src
BUILD_DIR = build

# List of source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# List of object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Your executable name
EXECUTABLE = shell

# Dependencies for installing required tools
INSTALL_DEPS = build-essential zip unzip libreadline-dev

.PHONY: all install clean

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

clean:
	rm -f $(EXECUTABLE) $(OBJ_FILES)

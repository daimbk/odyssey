CC = gcc
CFLAGS = -Wall -g
INCLUDES = -Iinclude
LIBS = -lreadline

SRC_DIR = src
BUILD_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# executable name
EXECUTABLE = shell

LINT_FILES = $(SRC_FILES)

TEST_SRC = test/*.c 
TEST_EXE = test/test

# dependencies for installing required tools
INSTALL_DEPS = build-essential zip unzip libreadline-dev

.PHONY: all install clean lint test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

lint: $(LINT_FILES)
	clang-format -i $(LINT_FILES)
    	clang-format -n $(LINT_FILES)
    	gcc -Wall -Werror $(LINT_FILES)

test: $(TEST_EXE)
    	./$(TEST_EXE)

$(TEST_EXE): $(TEST_SRC)
    	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

install:
	# Install necessary tools using the package manager
	sudo apt-get install -y $(INSTALL_DEPS)

clean:
	rm -f $(EXECUTABLE) $(OBJ_FILES) $(TEST_EXE)

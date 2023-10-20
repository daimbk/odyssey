CC = gcc
CFLAGS = -Wall -g

all: shell

shell: src/main.c
	$(CC) $(CFLAGS) -o shell src/main.c

clean:
	rm -f shell

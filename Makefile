CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = src/main.c
BIN = hello

.PHONY: all start clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

start: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

CC      = gcc
RAYLIB  = vendor/raylib/src
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude -I$(RAYLIB)

SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:src/%.c=build/%.o)
BIN     = hello

# test_sim: valida artemis.c/tiempo.c (fisica pura, sin raylib) por consola
TEST_BIN = test_sim
TEST_OBJ = build/artemis.o build/tiempo.o build/test_simulacion.o

# Librerías del sistema que necesita raylib
ifeq ($(OS),Windows_NT)
    LDLIBS = -lopengl32 -lgdi32 -lwinmm
    BIN   := $(BIN).exe
    TEST_BIN := $(TEST_BIN).exe
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Darwin)
        LDLIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    else
        LDLIBS = -lGL -lm -lpthread -ldl -lrt -lX11
    endif
endif

.PHONY: all start test clean cleanall

all: $(BIN)

# Enlazar el ejecutable
$(BIN): $(OBJ) $(RAYLIB)/libraylib.a
	$(CC) $(OBJ) $(RAYLIB)/libraylib.a -o $(BIN) $(LDLIBS)

# Compilar raylib (solo la primera vez)
$(RAYLIB)/libraylib.a:
	$(MAKE) -C $(RAYLIB) PLATFORM=PLATFORM_DESKTOP

# Compilar cada .c a build/*.o
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: tests/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

start: $(BIN)
	./$(BIN)

# Corre la fisica (artemis.c + tiempo.c) sin ventana, para verificarla antes de animarla
test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJ)
	$(CC) $(TEST_OBJ) -o $(TEST_BIN) -lm

clean:
	rm -rf build $(BIN) $(TEST_BIN)

cleanall: clean
	$(MAKE) -C $(RAYLIB) clean

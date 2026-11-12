CC      = gcc
RAYLIB  = vendor/raylib/src
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude -I$(RAYLIB)

SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:src/%.c=build/%.o)
BIN     = hello

# Librerías del sistema que necesita raylib
ifeq ($(OS),Windows_NT)
    LDLIBS = -lopengl32 -lgdi32 -lwinmm
    BIN   := $(BIN).exe
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Darwin)
        LDLIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    else
        LDLIBS = -lGL -lm -lpthread -ldl -lrt -lX11
    endif
endif

.PHONY: all start clean cleanall

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

build:
	mkdir -p build

start: $(BIN)
	./$(BIN)

clean:
	rm -rf build $(BIN)

cleanall: clean
	$(MAKE) -C $(RAYLIB) clean

CC := gcc

CFLAGS := -Wall -Wextra -Iinclude $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs)

DEPFLAGS := -MMD -MP

TARGET := chip8

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/%.o)
DEP := $(OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ) $(DEP)

-include $(DEP)

CC := gcc

CFLAGS := -Wall -Wextra $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs)

TARGET := chip8
SRC := ./main.c ./chip8.c ./operations.c ./graphics.c ./audio.c
OBJ := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

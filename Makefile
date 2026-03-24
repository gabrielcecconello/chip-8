CC := gcc
CFLAGS := -Wall -Wextra

TARGET := chip8
SRC := ./main.c ./chip8.c ./operations.c
OBJ := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

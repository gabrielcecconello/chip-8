#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include "graphics.h"
#include "definitions.h"

typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint8_t display[SCREEN_HEIGHT][SCREEN_WIDTH];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t last_key_pressed;
    uint8_t key_pressed_and_released;
    uint8_t keypad[KEYPAD_SIZE];
    uint8_t v_registers[NUM_OF_REGISTERS];
    uint16_t pc;
    uint16_t index_register;
    uint16_t stack[STACK_SIZE];
    uint16_t stack_pointer;
} Chip8;

void chip8_init(Chip8 *chip8);
int chip8_load_rom(Chip8 *chip8, const char *path);
void chip8_cycle(Chip8 *chip8, Graphics *graphics);
int chip8_process_events(SDL_Event *event, Chip8* chip8, Graphics* graphics);

#endif

#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <SDL2/SDL.h>
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
void chip8_load_rom(Chip8 *chip8);
uint16_t chip8_fetch(Chip8 *chip8);
void chip8_compute_key(Chip8 *chip8, SDL_Scancode scancode, uint8_t status);

#endif

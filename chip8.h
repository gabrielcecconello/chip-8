#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include "definitions.h"

typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint8_t display[SCREEN_WIDTH][SCREEN_HEIGHT];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t v_registers[16];
    uint16_t pc;
    uint16_t index_reg;
    uint16_t stack[16];
    uint16_t stack_pointer;
} Chip8;

void chip8_init(Chip8 *chip8);
void chip8_load_rom(Chip8 *chip8);
uint16_t chip8_fetch(Chip8 *chip8);

#endif

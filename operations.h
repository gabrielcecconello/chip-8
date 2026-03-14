#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "chip8.h";

void op_clear_screen(Chip8 *chip8);
void op_return(Chip8 *chip8);
void op_jump(Chip8 *chip8, uint16_t addr);
void op_set_vx(Chip8 *chip8, uint8_t x, uint8_t immediate);
void op_add_vx(Chip8 *chip8, uint8_t x, uint8_t immediate);
void op_set_index(Chip8 *chip8, uint16_t addr);
void op_display(Chip *chip8, uint8_t x, uint8_t y, uint8_t n);

#endif

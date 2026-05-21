#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>
#include "chip8.h"

void op_clear_screen(Chip8 *chip8);
void op_jump(Chip8 *chip8, uint16_t addr);
void op_call_subroutine(Chip8 *chip8, uint16_t addr);
void op_return(Chip8 *chip8);
void op_equal_immediate(Chip8 *chip8, uint8_t x, uint8_t immediate);
void op_not_equal_immediate(Chip8 *chip8, uint8_t x, uint8_t immediate);
void op_equal(Chip8 *chip8, uint8_t x, uint8_t y);
void op_not_equal(Chip8 *chip8, uint8_t x, uint8_t y);
void op_set_immediate(Chip8 *chip8, uint8_t x, uint8_t immediate);
void op_add_immediate(Chip8 *chip8, uint8_t x, uint8_t immediate);
void op_set(Chip8 *chip8, uint8_t x, uint8_t y);
void op_or(Chip8 *chip8, uint8_t x, uint8_t y);
void op_and(Chip8 *chip8, uint8_t x, uint8_t y);
void op_xor(Chip8 *chip8, uint8_t x, uint8_t y);
void op_add(Chip8 *chip8, uint8_t x, uint8_t y);
void op_subtract_xy(Chip8 *chip8, uint8_t x, uint8_t y);
void op_subtract_yx(Chip8 *chip8, uint8_t x, uint8_t y);
void op_shift_right(Chip8 *chip8, uint8_t x, uint8_t y);
void op_shift_left(Chip8 *chip8, uint8_t x, uint8_t y);
void op_set_index(Chip8 *chip8, uint16_t addr);
void op_display(Chip8 *chip8, uint8_t x, uint8_t y, uint8_t n);

#endif

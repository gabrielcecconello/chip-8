#include "operations.h"
#include "definitions.h"
#include "chip8.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void op_clear_screen(Chip8 *chip8) {
    memset(chip8->display, 0, sizeof(chip8->display));
}

void op_call_subroutine(Chip8 *chip8, uint16_t addr) {
    if (addr >= MEMORY_SIZE) {
        printf("Tried to access memory out of bounds.");
        return;
    }
    if (chip8->stack_pointer == 16) {
        printf("Stack overflow");
        return;
    }
    chip8->stack[chip8->stack_pointer] = chip8->pc;
    chip8->stack_pointer += 1;

    chip8->pc = addr;
}

void op_return(Chip8 *chip8) {
    if (chip8->stack_pointer == 0) {
        printf("Stack underflow.");
        return;
    }
    chip8->stack_pointer--;
    chip8->pc = chip8->stack[chip8->stack_pointer];
}

void op_jump(Chip8 *chip8, uint16_t addr) {
    if (addr >= MEMORY_SIZE) {
        printf("Tried to access memory out of bounds.");
        return;
    }
    chip8->pc = addr; 
}

void op_set_vx(Chip8 *chip8, uint8_t x, uint8_t immediate) {
    chip8->v_registers[x] = immediate;
}

void op_add_vx(Chip8 *chip8, uint8_t x, uint8_t immediate) {
    chip8->v_registers[x] += immediate;
}

void op_set_index(Chip8 *chip8, uint16_t addr) {
   chip8->index_register = addr;
}

static uint8_t extract_pixel(uint8_t sprite, int offset) {
    return (sprite >> (7 - offset)) & 1;
}

void op_display(Chip8 *chip8, uint8_t x, uint8_t y, uint8_t n) {
    uint8_t x_coordinate = chip8->v_registers[x] % SCREEN_WIDTH;
    uint8_t y_coordinate = chip8->v_registers[y] % SCREEN_HEIGHT;
    
    chip8->v_registers[VF] = 0;

    for (uint8_t i = 0; i < n && y_coordinate + i < SCREEN_HEIGHT; i++) {
        uint8_t sprite = chip8->memory[chip8->index_register + i];

        for (int j = 0; j < 8 && x_coordinate + j < SCREEN_WIDTH; j++) {
            uint8_t pixel = extract_pixel(sprite, j);

            if (pixel == 1) {
                uint8_t *current_pixel = &chip8->display[y_coordinate + i][x_coordinate + j];
                uint8_t old_value = *current_pixel;

                *current_pixel ^= pixel;
                if (old_value && !(*current_pixel)) {
                    chip8->v_registers[VF] = 1; 
                }
            }
        }
    }
}

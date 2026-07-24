#include "operations.h"
#include "definitions.h"
#include "chip8.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void op_clear_screen(Chip8 *chip8) {
    memset(chip8->display, 0, sizeof(chip8->display));
}

void op_jump(Chip8 *chip8, uint16_t addr) {
    if (addr >= MEMORY_SIZE) {
        fprintf(stderr, "op_jump: address 0x%03X is out of bounds.\n", addr);
        return;
    }
    chip8->pc = addr; 
}

void op_call_subroutine(Chip8 *chip8, uint16_t addr) {
    if (addr >= MEMORY_SIZE) {
        fprintf(stderr, "op_call_subroutine: address 0x%03X is out of bounds.\n", addr);
        return;
    }
    if (chip8->stack_pointer == STACK_SIZE) {
        fprintf(stderr, "op_call_subroutine: stack overflow.\n");
        return;
    }
    chip8->stack[chip8->stack_pointer] = chip8->pc;
    chip8->stack_pointer += 1;

    chip8->pc = addr;
}

void op_return(Chip8 *chip8) {
    if (chip8->stack_pointer == 0) {
        fprintf(stderr, "op_return: stack underflow.\n");
        return;
    }
    chip8->stack_pointer--;
    chip8->pc = chip8->stack[chip8->stack_pointer];
}

void skip_instruction(Chip8 *chip8) {
    chip8->pc += 2;
}

void op_equal_immediate(Chip8 *chip8, uint8_t x, uint8_t immediate) {
    if (chip8->v_registers[x] == immediate) {
        skip_instruction(chip8);
    }
}

void op_not_equal_immediate(Chip8 *chip8, uint8_t x, uint8_t immediate) {
    if (chip8->v_registers[x] != immediate) {
        skip_instruction(chip8);
    }
}

void op_equal(Chip8 *chip8, uint8_t x, uint8_t y) {
    if (chip8->v_registers[x] == chip8->v_registers[y]) {
        skip_instruction(chip8);
    }
}

void op_not_equal(Chip8 *chip8, uint8_t x, uint8_t y) {
    if (chip8->v_registers[x] != chip8->v_registers[y]) {
        skip_instruction(chip8);
    }
}

void op_set_immediate(Chip8 *chip8, uint8_t x, uint8_t immediate) {
    chip8->v_registers[x] = immediate;
}

void op_add_immediate(Chip8 *chip8, uint8_t x, uint8_t immediate) {
    chip8->v_registers[x] += immediate;
}

void op_set(Chip8 *chip8, uint8_t x, uint8_t y) {
    chip8->v_registers[x] = chip8->v_registers[y];
}

void op_or(Chip8 *chip8, uint8_t x, uint8_t y) {
    chip8->v_registers[x] = chip8->v_registers[x] | chip8->v_registers[y];
}

void op_and(Chip8 *chip8, uint8_t x, uint8_t y) {
    chip8->v_registers[x] = chip8->v_registers[x] & chip8->v_registers[y];
}

void op_xor(Chip8 *chip8, uint8_t x, uint8_t y) {
    chip8->v_registers[x] = chip8->v_registers[x] ^ chip8->v_registers[y];
}

void op_add(Chip8 *chip8, uint8_t x, uint8_t y) {
    uint8_t vx = chip8->v_registers[x];
    uint8_t vy = chip8->v_registers[y];

    chip8->v_registers[VF] = (vx + vy) > UINT8_MAX;
    chip8->v_registers[x] = vx + vy;
}

void op_subtract_xy(Chip8 *chip8, uint8_t x, uint8_t y) {
    uint8_t vx = chip8->v_registers[x];
    uint8_t vy = chip8->v_registers[y];

    chip8->v_registers[VF] = !(vx < vy);
    chip8->v_registers[x] = vx - vy;
}

void op_subtract_yx(Chip8 *chip8, uint8_t x, uint8_t y) {
    uint8_t vx = chip8->v_registers[x];
    uint8_t vy = chip8->v_registers[y];

    chip8->v_registers[VF] = !(vy < vx);
    chip8->v_registers[x] = vy - vx;
}

void op_shift_right(Chip8 *chip8, uint8_t x, uint8_t y) {
    uint8_t vy = chip8->v_registers[y];

    chip8->v_registers[VF] = vy & 0x01;
    chip8->v_registers[x] = vy >> 1;
}

void op_shift_left(Chip8 *chip8, uint8_t x, uint8_t y) {
    uint8_t vy = chip8->v_registers[y];

    chip8->v_registers[VF] = (vy & 0x80) >> 7;
    chip8->v_registers[x] = vy << 1;
}

void op_set_index(Chip8 *chip8, uint16_t addr) {
   chip8->index_register = addr;
}

void op_jump_offset(Chip8 *chip8, uint16_t addr) {
    uint16_t offset_sum = addr + chip8->v_registers[0];

    if (offset_sum >= MEMORY_SIZE) {
        fprintf(stderr, "op_jump_offset: address 0x%03X is out of bounds.\n", offset_sum);
        return;
    }
    chip8->pc = offset_sum;
}

void op_random(Chip8 *chip8, uint8_t x, uint8_t immediate) {
    chip8->v_registers[x] = (rand() % (UINT8_MAX + 1)) & immediate;   
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

void skip_key_aux(Chip8 *chip8, uint8_t x, uint8_t expected_state) {
    uint8_t key = chip8->v_registers[x];
    if (key > 0xF) {
        fprintf(stderr, "skip_key_aux: invalid key value 0x%02X.\n", key);
        return;
    }

    if (chip8->keypad[key] == expected_state) {
        skip_instruction(chip8);
    }
}

void op_skip_key_pressed(Chip8 *chip8, uint8_t x) {
    skip_key_aux(chip8, x, 1);
}

void op_skip_key_not_pressed(Chip8 *chip8, uint8_t x) {
    skip_key_aux(chip8, x, 0);
}

void op_get_key(Chip8 *chip8, uint8_t x) {
    if (!chip8->key_pressed_and_released) {
        chip8->pc -= 2;
        return;
    }

    chip8->v_registers[x] = chip8->last_key_pressed;

    chip8->key_pressed_and_released = 0;
    chip8->last_key_pressed = INVALID_KEY;
}

void op_add_index(Chip8 *chip8, uint8_t x) {
    chip8->index_register += chip8->v_registers[x];
}

void op_font_character(Chip8 *chip8, uint8_t x) {
    uint8_t character = chip8->v_registers[x] & 0x0F;
    chip8->index_register = FONT_START + (character * FONT_HEIGHT);
}

void op_binary_decimal(Chip8 *chip8, uint8_t x) {
    uint8_t vx = chip8->v_registers[x];
    uint16_t i = chip8->index_register;

    if (i > MEMORY_SIZE - 3) {
        fprintf(stderr, "op_binary_decimal: conversion would write beyond memory bounds (i = 0x%03X).\n", i);
        return;
    }

    chip8->memory[i] = vx / 100;
    chip8->memory[i + 1] = (vx / 10) % 10;
    chip8->memory[i + 2] = vx % 10;
}

void op_store_memory(Chip8 *chip8, uint8_t x) {
    uint8_t num_of_variables = x + 1;

    if (chip8->index_register > MEMORY_SIZE - num_of_variables) {
        fprintf(stderr, "op_store_memory: storing %d variables in memory would write beyond bounds (i = 0x%03X).\n", num_of_variables, chip8->index_register);
        return;
    }

    for (uint8_t i = 0; i <= x; i++) {
        chip8->memory[chip8->index_register] = chip8->v_registers[i];
        chip8->index_register++;
    }
}

void op_load_memory(Chip8 *chip8, uint8_t x) {
    uint8_t num_of_variables = x + 1;

    if (chip8->index_register > MEMORY_SIZE - num_of_variables) {
        fprintf(stderr, "op_load_memory: loading %d variables from memory would read beyond bounds (i = 0x%03X).\n", num_of_variables, chip8->index_register);
        return;
    }

    for (uint8_t i = 0; i <= x; i++) {
        chip8->v_registers[i] = chip8->memory[chip8->index_register];
        chip8->index_register++;
    }
}

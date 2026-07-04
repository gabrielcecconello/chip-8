#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "definitions.h"

static const uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init(Chip8 *chip8) {
    memset(chip8->memory, 0, sizeof(chip8->memory));
    chip8->pc = START_ADDRESS;
    chip8->index_register = 0;
   
    memset(chip8->v_registers, 0, sizeof(chip8->v_registers));
    
    memset(chip8->stack, 0, sizeof(chip8->stack));
    chip8->stack_pointer = 0;

    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    chip8->last_key_pressed = INVALID_KEY;
    chip8->key_pressed_and_released = 0;

    memset(chip8->keypad, 0, sizeof(chip8->keypad));

    memset(chip8->display, 0, sizeof(chip8->display));

    // Initializes part of the memory with the established fontset
    for (size_t i = 0; i < sizeof(fontset); i++) {
        chip8->memory[0x50 + i] = fontset[i];
    }
}

static size_t get_rom_size(FILE *rom) {
    fseek(rom, 0, SEEK_END);
    size_t rom_size = (size_t) ftell(rom);
    rewind(rom);
    return rom_size;
}

int chip8_load_rom(Chip8 *chip8, const char *path) {
    FILE *rom = fopen(path, "rb");
    if (!rom) {
        return 1;
    }
   
    size_t rom_size = get_rom_size(rom);
    size_t max_program_size = sizeof(chip8->memory) - START_ADDRESS;

    if (rom_size > max_program_size) {
        fclose(rom);
        return 1;
    }

    size_t bytes_read = fread(&chip8->memory[START_ADDRESS], sizeof(uint8_t), rom_size, rom);
    if (bytes_read != rom_size) {
        fclose(rom);
        return 1;
    }

    fclose(rom);
    return 0;
}

uint16_t chip8_fetch(Chip8 *chip8) {
    uint16_t pc = chip8->pc;

    uint16_t opcode =
        (chip8->memory[pc] << 8) |
        chip8->memory[pc + 1];

    chip8->pc += 2;
    return opcode;
}

static void chip8_track_key(Chip8 *chip8, uint8_t status, uint8_t key) {
    if (status) {
        // Starts tracking a key if a key isn't already being tracked
        if (chip8->last_key_pressed == INVALID_KEY) {
            chip8->last_key_pressed = key;
        }
    }
    else {
        // Computes the event if the key being tracked was released
        if (chip8->last_key_pressed == key) {
            chip8->key_pressed_and_released = 1; 
        }
    }
}

void chip8_compute_key(Chip8 *chip8, SDL_Scancode scancode, uint8_t status) {
    uint8_t key = INVALID_KEY;

    switch (scancode) {
        case SDL_SCANCODE_1:
            key = 0x1;
            break;
        case SDL_SCANCODE_2:
            key = 0x2;
            break;
        case SDL_SCANCODE_3:
            key = 0x3;
            break;
        case SDL_SCANCODE_4:
            key = 0xC;
            break;
        case SDL_SCANCODE_Q:
            key = 0x4;
            break;
        case SDL_SCANCODE_W:
            key = 0x5;
            break;
        case SDL_SCANCODE_E:
            key = 0x6;
            break;
        case SDL_SCANCODE_R:
            key = 0xD;
            break;
        case SDL_SCANCODE_A:
            key = 0x7;
            break;
        case SDL_SCANCODE_S:
            key = 0x8;
            break;
        case SDL_SCANCODE_D:
            key = 0x9;
            break;
        case SDL_SCANCODE_F:
            key = 0xE;
            break;
        case SDL_SCANCODE_Z:
            key = 0xA;
            break;
        case SDL_SCANCODE_X:
            key = 0x0;
            break;
        case SDL_SCANCODE_C:
            key = 0xB;
            break;
        case SDL_SCANCODE_V:
            key = 0xF;
            break;
        default:
            break;
    }

    if (key != INVALID_KEY) {
        chip8->keypad[key] = status;
        chip8_track_key(chip8, status, key);
    }
}

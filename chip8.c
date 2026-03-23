#include <stdint.h>
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

size_t get_rom_size(FILE *rom) {
    fseek(rom, 0, SEEK_END);
    size_t rom_size = (size_t) ftell(rom);
    rewind(rom);
    return rom_size;
}

static const char* rom_path = "./roms/ibm_logo.ch8";

void chip8_init(Chip8 *chip8) {
    memset(chip8->memory, 0, sizeof(chip8->memory));
    chip8->pc = START_ADDRESS;
    chip8->index_register = 0;
   
    memset(chip8->v_registers, 0, sizeof(chip8->v_registers));
    
    memset(chip8->stack, 0, sizeof(chip8->stack));
    chip8->stack_pointer = 0;

    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    memset(chip8->display, 0, sizeof(chip8->display));

    // Initializes part of the memory with the established fontset
    for (size_t i = 0; i < sizeof(fontset); i++) {
        chip8->memory[0x50 + i] = fontset[i];
    }
}

void chip8_load_rom(Chip8 *chip8) {
    FILE *rom = fopen(rom_path, "rb");
    if (!rom) {
        printf("Failed to open ROM.\n");
        return;
    }
   
    size_t rom_size = get_rom_size(rom);
    int max_program_size = sizeof(chip8->memory) - START_ADDRESS;

    if (rom_size > max_program_size) {
        printf("ROM size is bigger than free memory space.\n");
        return;
    }

    fread(&chip8->memory[START_ADDRESS], sizeof(uint8_t), rom_size, rom);
    fclose(rom);
}

uint16_t chip8_fetch(Chip8 *chip8) {
    uint16_t pc = chip8->pc;

    uint16_t opcode =
        (chip8->memory[pc] << 8) |
        chip8->memory[pc + 1];

    chip8->pc += 2;
    return opcode;
}

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "graphics.h"
#include "operations.h"
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
        chip8->memory[FONT_START + i] = fontset[i];
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

static uint16_t chip8_fetch(Chip8 *chip8) {
    uint16_t pc = chip8->pc;

    uint16_t opcode =
        (chip8->memory[pc] << 8) |
        chip8->memory[pc + 1];

    chip8->pc += 2;
    return opcode;
}

void chip8_cycle(Chip8 *chip8, Graphics *graphics) {
    uint16_t opcode = chip8_fetch(chip8);

    // Extracting nibbles from current instruction
    uint8_t n1 = (opcode & 0xF000) >> 12;
    uint8_t n2 = (opcode & 0x0F00) >> 8;
    uint8_t n3 = (opcode & 0x00F0) >> 4;
    uint8_t n4 = opcode & 0x000F;

    // Extracting combination of nibbles for certain operations
    uint8_t nn = opcode & 0x00FF;
    uint16_t nnn = opcode & 0x0FFF;

    // Decode/Execute switch block
    switch (n1) {
        case 0x0:
            switch (nn) {
                case 0xE0:
                    op_clear_screen(chip8);
                    break;
                case 0xEE:
                    op_return(chip8);
                    break;
                default:
                    break;
            }
            break;
        case 0x1:
            op_jump(chip8, nnn);
            break;
        case 0x2:
            op_call_subroutine(chip8, nnn);
            break;
        case 0x3:
            op_equal_immediate(chip8, n2, nn);
            break;
        case 0x4:
            op_not_equal_immediate(chip8, n2, nn);
            break;
        case 0x5:
            op_equal(chip8, n2, n3);
            break;
        case 0x6:
            op_set_immediate(chip8, n2, nn);
            break;
        case 0x7:
            op_add_immediate(chip8, n2, nn);
            break;
        case 0x8:
            switch (n4) {
                case 0x0:
                    op_set(chip8, n2, n3);
                    break;
                case 0x1:
                    op_or(chip8, n2, n3);
                    break;
                case 0x2:
                    op_and(chip8, n2, n3);
                    break;
                case 0x3:
                    op_xor(chip8, n2, n3);
                    break;
                case 0x4:
                    op_add(chip8, n2, n3);
                    break;
                case 0x5:
                    op_subtract_xy(chip8, n2, n3);
                    break;
                case 0x6:
                    op_shift_right(chip8, n2, n3);
                    break;
                case 0x7:
                    op_subtract_yx(chip8, n2, n3);
                    break;
                case 0xE:
                    op_shift_left(chip8, n2, n3);
                    break;
                default:
                    break;
            }
            break;
        case 0x9:
            op_not_equal(chip8, n2, n3);
            break;
        case 0xA:
            op_set_index(chip8, nnn);
            break;
        case 0xB:
            op_jump_offset(chip8, nnn);
            break;
        case 0xC:
            op_random(chip8, n2, nn);
            break;
        case 0xD:
            op_display(chip8, n2, n3, n4);
            graphics_draw(graphics, chip8->display);
            SDL_RenderPresent(graphics->renderer);
            break;
        case 0xE:
            switch (n3) {
                case 0x9:
                    op_skip_key_pressed(chip8, n2);
                    break;
                case 0xA:
                    op_skip_key_not_pressed(chip8, n2);
                    break;
                default:
                    break;
            }
            break;
        case 0xF:
            switch (nn) {
                case 0x07:
                    op_set_to_delay(chip8, n2);
                    break;
                case 0x0A:
                    op_get_key(chip8, n2);
                    break;
                case 0x15:
                    op_set_delay(chip8, n2);
                    break;
                case 0x18:
                    op_set_sound(chip8, n2);
                    break;
                case 0x1E:
                    op_add_index(chip8, n2);
                    break;
                case 0x29:
                    op_font_character(chip8, n2);
                    break;
                case 0x33:
                    op_binary_decimal(chip8, n2);
                    break;
                case 0x55:
                    op_store_memory(chip8, n2);
                    break;
                case 0x65:
                    op_load_memory(chip8, n2);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
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

static void chip8_compute_key(Chip8 *chip8, SDL_Scancode scancode, uint8_t status) {
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

int chip8_process_events(SDL_Event *event, Chip8 *chip8, Graphics *graphics) {
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                graphics_destroy(graphics);
                return 1;
            case SDL_WINDOWEVENT:
                if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
                    graphics_draw(graphics, chip8->display);
                }
                break;
            case SDL_KEYDOWN:
                chip8_compute_key(chip8, event->key.keysym.scancode, 1);
                break;
            case SDL_KEYUP:
                chip8_compute_key(chip8, event->key.keysym.scancode, 0);
                break;
            default:
                break;
        }
    }

    return 0;
}

void chip8_update_timers(Chip8 *chip8) {
    if (chip8->delay_timer > 0) {
        chip8->delay_timer--;
    }

    if (chip8->sound_timer > 0) {
        chip8->sound_timer--;
    }   
}

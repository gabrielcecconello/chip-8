#include <SDL2/SDL.h>
#include <stdint.h>
#include "chip8.h"
#include "graphics.h"
#include "operations.h"

Chip8 chip8;
Graphics graphics;

int main() {
    chip8_init(&chip8);
    chip8_load_rom(&chip8);

    if (graphics_init(&graphics)) {
        graphics_destroy(&graphics);
        return 0;
    }

    SDL_Event event;

    // Fetch/Decode/Execute loop
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    graphics_destroy(&graphics);
                    return 0;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        graphics_draw(&graphics, chip8.display);
                    }
                    break;
                case SDL_KEYDOWN:
                    chip8_compute_key(&chip8, event.key.keysym.scancode, 1);
                    break;
                case SDL_KEYUP:
                    chip8_compute_key(&chip8, event.key.keysym.scancode, 0);
                    break;
                default:
                    break;
            }
        }

        uint16_t opcode = chip8_fetch(&chip8);

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
                switch (nnn) {
                    case 0x0E0:
                        op_clear_screen(&chip8);
                        break;
                    case 0x0EE:
                        op_return(&chip8);
                        break;
                    default:
                        break;
                }
                break;
            case 0x1:
                op_jump(&chip8, nnn);
                break;
            case 0x2:
                op_call_subroutine(&chip8, nnn);
                break;
            case 0x3:
                op_equal_immediate(&chip8, n2, nn);
                break;
            case 0x4:
                op_not_equal_immediate(&chip8, n2, nn);
                break;
            case 0x5:
                op_equal(&chip8, n2, n3);
                break;
            case 0x6:
                op_set_immediate(&chip8, n2, nn);
                break;
            case 0x7:
                op_add_immediate(&chip8, n2, nn);
                break;
            case 0x8:
                switch (n4) {
                    case 0x0:
                        op_set(&chip8, n2, n3);
                        break;
                    case 0x1:
                        op_or(&chip8, n2, n3);
                        break;
                    case 0x2:
                        op_and(&chip8, n2, n3);
                        break;
                    case 0x3:
                        op_xor(&chip8, n2, n3);
                        break;
                    case 0x4:
                        op_add(&chip8, n2, n3);
                        break;
                    case 0x5:
                        op_subtract_xy(&chip8, n2, n3);
                        break;
                    case 0x6:
                        op_shift_right(&chip8, n2, n3);
                        break;
                    case 0x7:
                        op_subtract_yx(&chip8, n2, n3);
                        break;
                    case 0xE:
                        op_shift_left(&chip8, n2, n3);
                    default:
                        break;
                }
                break;
            case 0x9:
                op_not_equal(&chip8, n2, n3);
                break;
            case 0xA:
                op_set_index(&chip8, nnn);
                break;
            case 0xB:
                op_jump_offset(&chip8, nnn);
                break;
            case 0xC:
                op_random(&chip8, n2, nn);
                break;
            case 0xD:
                op_display(&chip8, n2, n3, n4);
                graphics_draw(&graphics, chip8.display);
                break;
            default:
                break;
        }
        
        SDL_RenderPresent(graphics.renderer);
        SDL_Delay(16);
    }
}

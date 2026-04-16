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

    graphics_init(&graphics);

    SDL_Event event;

    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            }
        }

        SDL_Delay(16);
    }

    // // Fetch/Decode/Execute loop
    // while (1) {
    //     uint16_t opcode = chip8_fetch(&chip8);
    //
    //     // Extracting nibbles from current instruction
    //     uint8_t n1 = (opcode & 0xF000) >> 12;
    //     uint8_t n2 = (opcode & 0x0F00) >> 8;
    //     uint8_t n3 = (opcode & 0x00F0) >> 4;
    //     uint8_t n4 = opcode & 0x000F;
    //
    //     // Extracting combination of nibbles for certain operations
    //     uint8_t nn = opcode & 0x00FF;
    //     uint16_t nnn = opcode & 0x0FFF;
    //
    //     // Decode/Execute switch block
    //     switch (n1) {
    //         case 0x0:
    //             switch (nnn) {
    //                 case 0x0E0:
    //                     op_clear_screen(&chip8);
    //                     break;
    //                 case 0x0EE:
    //                     op_return(&chip8);
    //                     break;
    //                 default:
    //                     break;
    //             }
    //             break;
    //         case 0x1:
    //             op_jump(&chip8, nnn);
    //             break;
    //         case 0x6:
    //             op_set_vx(&chip8, n2, nn);
    //             break;
    //         case 0x7:
    //             op_add_vx(&chip8, n2, nn);
    //             break;
    //         case 0xA:
    //             op_set_index(&chip8, nnn);
    //             break;
    //         case 0xD:
    //             op_display(&chip8, n2, n3, n4);
    //             break;
    //         default:
    //             break;
    //     }
    // }
}

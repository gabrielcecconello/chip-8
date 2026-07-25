#include <SDL2/SDL.h>
#include <stdint.h>
#include "chip8.h"
#include "graphics.h"

Chip8 chip8;
Graphics graphics;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s *rom_path*\n", argv[0]);
        return 1;
    }
    
    chip8_init(&chip8);
    
    if (chip8_load_rom(&chip8, argv[1])) {
        perror("Failed to open ROM");
        return 1;
    }

    if (graphics_init(&graphics)) {
        graphics_destroy(&graphics);
        return 1;
    }

    SDL_Event event;

    // Fetch/Decode/Execute loop
    while (1) {
        if (chip8_process_events(&event, &chip8, &graphics)) {
            break;
        }

        chip8_cycle(&chip8, &graphics);

        SDL_Delay(16);
    }

    return 0;
}

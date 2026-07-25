#include <SDL2/SDL.h>
#include <stdint.h>
#include "chip8.h"
#include "graphics.h"

Chip8 chip8;
Graphics graphics;

const double CPU_HZ = 700.0;
const double CPU_PERIOD = 1.0 / CPU_HZ;

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

    double seconds_elapsed;
    double accumulator = 0.0;

    uint64_t current_counter;
    uint64_t last_counter = SDL_GetPerformanceCounter();

    // Fetch/Decode/Execute loop
    while (1) {
        current_counter = SDL_GetPerformanceCounter();

        seconds_elapsed = (double) (current_counter - last_counter) / SDL_GetPerformanceFrequency();

        last_counter = current_counter;

        accumulator += seconds_elapsed;
        
        if (chip8_process_events(&event, &chip8, &graphics)) {
            break;
        }

        while (accumulator >= CPU_PERIOD) {
            chip8_cycle(&chip8, &graphics);
            accumulator -= CPU_PERIOD;
        }
    }

    return 0;
}

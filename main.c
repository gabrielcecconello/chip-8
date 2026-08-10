#include <SDL2/SDL.h>
#include <stdint.h>
#include "chip8.h"
#include "graphics.h"

Chip8 chip8;
Graphics graphics;

const double CPU_HZ = 700.0;
const double CPU_PERIOD = 1.0 / CPU_HZ;

const double TIMER_HZ = 60.0;
const double TIMER_PERIOD = 1.0 / TIMER_HZ;

const uint32_t subsystem_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;

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

    if (SDL_InitSubSystem(subsystem_flags) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    if (graphics_init(&graphics)) {
        graphics_destroy(&graphics);
        SDL_QuitSubSystem(subsystem_flags);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;

    double seconds_elapsed;
    double cpu_accumulator = 0.0;
    double timers_accumulator = 0.0;

    uint64_t current_counter;
    uint64_t last_counter = SDL_GetPerformanceCounter();

    // Fetch/Decode/Execute loop
    while (1) {
        current_counter = SDL_GetPerformanceCounter();

        seconds_elapsed = (double) (current_counter - last_counter) / SDL_GetPerformanceFrequency();

        last_counter = current_counter;

        cpu_accumulator += seconds_elapsed;
        timers_accumulator += seconds_elapsed;
            
        if (chip8_process_events(&event, &chip8, &graphics)) {
            break;
        }

        while (cpu_accumulator >= CPU_PERIOD) {
            chip8_cycle(&chip8, &graphics);
            cpu_accumulator -= CPU_PERIOD;
        }

        while (timers_accumulator >= TIMER_PERIOD) {
            chip8_update_timers(&chip8);
            timers_accumulator -= TIMER_PERIOD;
        }
    }

    return 0;
}

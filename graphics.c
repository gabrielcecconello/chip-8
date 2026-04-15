#include <SDL2/SDL.h>
#include "graphics.h"
#include "definitions.h"

void init_window() {
    SDL_InitSubSystem(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                        SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
}

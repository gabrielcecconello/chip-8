#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "graphics.h"
#include "definitions.h"

static const uint32_t window_flags = SDL_WINDOW_RESIZABLE;
static const uint32_t renderer_flags = SDL_RENDERER_ACCELERATED;

void graphics_init(Graphics *graphics) {
    memset(graphics->raw_pixels, 0, sizeof(graphics->raw_pixels));

    SDL_InitSubSystem(SDL_INIT_TIMER | SDL_INIT_VIDEO);

    // Window is where the image is displayed
    graphics->window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    800, 600, window_flags);
    
    // Renderer is what draws the image
    graphics->renderer = SDL_CreateRenderer(graphics->window, -1, renderer_flags);

    // Setting renderer's logical size to allow automatic scalling
    SDL_RenderSetLogicalSize(graphics->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Texture is the image that it's drawn 
    graphics->texture = SDL_CreateTexture(graphics->renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void graphics_draw(Graphics *graphics, uint8_t display[SCREEN_HEIGHT][SCREEN_WIDTH]) {
    // Populating raw pixels buffer with display's current state
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int index = y * SCREEN_WIDTH + x;
            graphics->raw_pixels[index] = display[y][x] ? WHITE_PIXEL : BLACK_PIXEL;
        }
    }
    
    // Draws and refreshes the screen  
    SDL_UpdateTexture(graphics->texture, NULL, graphics->raw_pixels, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(graphics->renderer, graphics->texture, NULL, NULL);
    SDL_RenderPresent(graphics->renderer);
}

#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "graphics.h"
#include "definitions.h"

static const uint32_t window_flags = SDL_WINDOW_RESIZABLE;
static const uint32_t renderer_flags = SDL_RENDERER_ACCELERATED;

int graphics_init(Graphics *graphics) {
    memset(graphics->raw_pixels, 0, sizeof(graphics->raw_pixels));

    // Window is where the image is displayed
    graphics->window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    800, 600, window_flags);
    if (!graphics->window) return 1;
    
    // Renderer is what draws the image
    graphics->renderer = SDL_CreateRenderer(graphics->window, -1, renderer_flags);
    if (!graphics->renderer) return 1;

    // Setting renderer's logical size to allow automatic scalling
    SDL_RenderSetLogicalSize(graphics->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(graphics->renderer, 0, 0, 0, 255);
    
    // Texture is the image that it's drawn 
    graphics->texture = SDL_CreateTexture(graphics->renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!graphics->texture) return 1;

    return 0;
}

void graphics_draw(Graphics *graphics, uint8_t display[SCREEN_HEIGHT][SCREEN_WIDTH]) {
    // Populating raw pixels buffer with display's current state
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int index = y * SCREEN_WIDTH + x;
            graphics->raw_pixels[index] = display[y][x] ? WHITE_PIXEL : BLACK_PIXEL;
        }
    }
    
    // Clears and draws to renderer  
    SDL_UpdateTexture(graphics->texture, NULL, graphics->raw_pixels, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(graphics->renderer);
    SDL_RenderCopy(graphics->renderer, graphics->texture, NULL, NULL);
    SDL_RenderPresent(graphics->renderer);
}

void graphics_destroy(Graphics *graphics) {
    SDL_DestroyTexture(graphics->texture);
    graphics->texture = NULL;

    SDL_DestroyRenderer(graphics->renderer);
    graphics->renderer = NULL;

    SDL_DestroyWindow(graphics->window);
    graphics->window = NULL;
}

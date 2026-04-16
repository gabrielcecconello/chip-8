#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "definitions.h"
#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint32_t raw_pixels[SCREEN_HEIGHT * SCREEN_WIDTH];
} Graphics;

void graphics_init(Graphics *graphics);
void graphics_draw(Graphics *graphics, uint8_t display[SCREEN_HEIGHT][SCREEN_WIDTH]);

#endif

#ifndef AUDIO_H
#define AUDIO_H

#include "chip8.h"
#include <SDL2/SDL.h>

int audio_init(Chip8 *chip8);
void audio_destroy();

#endif

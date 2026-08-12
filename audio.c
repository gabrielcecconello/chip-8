#include "audio.h"
#include "chip8.h"
#include <SDL2/SDL.h>

static const int AUDIO_SAMPLE_RATE = 44100;
static const Uint16 AUDIO_BUFFER_SIZE = 1024;
static const double AUDIO_FREQUENCY = 440.0;
static const int16_t AUDIO_AMPLITUDE = 10000;

static double phase = 0.0;
static SDL_AudioDeviceID audio_device = 0;

static void updatePhase() {
    phase += AUDIO_FREQUENCY / AUDIO_SAMPLE_RATE;

    if (phase >= 1.0) phase -= 1.0;
}

static void audio_generate_square_wave(Uint8 *stream, int len) {
    int16_t *samples = (int16_t *) stream;
    int num_samples = len / sizeof(int16_t);

    for (int i = 0; i < num_samples; i++) {
        samples[i] = phase < 0.5 ? AUDIO_AMPLITUDE : -AUDIO_AMPLITUDE;
        updatePhase();
    }
}

static void audio_callback(void* userdata, Uint8 *stream, int len) {
    Chip8 *chip8 = userdata;

    if (chip8->sound_timer == 0) {
        memset(stream, 0, len); // Generates silence
        return;
    }

    audio_generate_square_wave(stream, len); 
}

int audio_init(Chip8 *chip8)  {
    SDL_AudioSpec desired_audio_spec = {
        .freq = AUDIO_SAMPLE_RATE,
        .format = AUDIO_S16SYS,
        .channels = 1,
        .samples = AUDIO_BUFFER_SIZE,
        .callback = audio_callback,
        .userdata = chip8
    };

    SDL_AudioSpec obtained_audio_spec;

    audio_device = SDL_OpenAudioDevice(NULL, 0, &desired_audio_spec, &obtained_audio_spec, 0);

    if (!audio_device) return 1;

    SDL_PauseAudioDevice(audio_device, 0);

    return 0;
}

void audio_destroy() {
    if (audio_device != 0) {
        SDL_CloseAudioDevice(audio_device);
        audio_device = 0;
    }
}


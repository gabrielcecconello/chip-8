#include "audio.h"
#include "chip8.h"
#include <SDL2/SDL.h>

static const int AUDIO_SAMPLE_RATE = 44100;
static const Uint16 AUDIO_BUFFER_SIZE = 1024;
static const double AUDIO_FREQUENCY = 440.0;
static const int16_t AUDIO_AMPLITUDE = 5000;

static double phase = 0.0;
static SDL_AudioStream *audio_stream;
static SDL_AudioDeviceID audio_device = 0;

static void updatePhase() {
    phase += AUDIO_FREQUENCY / AUDIO_SAMPLE_RATE;

    if (phase >= 1.0) phase -= 1.0;
}

// No matter how the audio device was configured, it generates a 44.1 KHz / Mono / S16 square wave
static void audio_generate_square_wave(int16_t *samples, int num_samples) {
    for (int i = 0; i < num_samples; i++) {
        samples[i] = phase < 0.5 ? AUDIO_AMPLITUDE : -AUDIO_AMPLITUDE;
        updatePhase();
    }
}

static void audio_callback(void *userdata, Uint8 *stream, int len) {
    Chip8 *chip8 = userdata;

    if (chip8->sound_timer == 0) {
        memset(stream, 0, len); // Generates silence
        return;
    }

    int num_samples = len / sizeof(int16_t);
    int16_t samples[num_samples];
    
    audio_generate_square_wave(samples, num_samples);
    
    // It adjusts the generated square wave to meet the defined audio device specs
    SDL_AudioStreamPut(audio_stream, (void *) samples, len);
    int bytes_obtained = SDL_AudioStreamGet(audio_stream, (void *) stream, len);

    if (bytes_obtained < len) {
        memset(stream + bytes_obtained, 0, len - bytes_obtained); // Fills the rest of the stream with silence
    }
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

    audio_device = SDL_OpenAudioDevice(
        NULL,                         // Using default audio device
        0,                            // Setting audio device to playback
        &desired_audio_spec,          //
        &obtained_audio_spec,         //
        SDL_AUDIO_ALLOW_ANY_CHANGE    // SDL changes any specs if system doesn't meet the desired audio specs
    );

    if (!audio_device) {
        fprintf(stderr, "audio_init: failed to initialize audio device %s\n", SDL_GetError());
        return 1;
    }

    audio_stream = SDL_NewAudioStream(
        AUDIO_S16SYS,                 // Source audio format
        1,                            // Source audio number of channels
        AUDIO_SAMPLE_RATE,            // Source audio sample rate
        obtained_audio_spec.format,   // Output audio format
        obtained_audio_spec.channels, // Output audio number of channels
        obtained_audio_spec.freq      // Output audio sample rate
                
    );

    if (!audio_stream) {
        fprintf(stderr, "audio_init: failed to create audio stream %s\n", SDL_GetError());
        return 1;
    }

    SDL_PauseAudioDevice(audio_device, 0);

    return 0;
}

void audio_destroy() {
    if (audio_stream) {
        SDL_FreeAudioStream(audio_stream);
        audio_stream = NULL;
    }

    if (audio_device != 0) {
        SDL_CloseAudioDevice(audio_device);
        audio_device = 0;
    }
}

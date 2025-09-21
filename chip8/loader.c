#include "chip8.h"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_timer.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void chip8_load(Chip8 *chip8, const char *rom_path) {
  FILE *rom = fopen(rom_path, "rb");
  if (!rom) {
    perror("Failed to open ROM");
    exit(1);
  }

  printf("starting to load the rom\n");
  int pos = 0x200; // programs start at 0x200
  int c;
  while ((c = fgetc(rom)) != EOF && pos < 0x1000) {
    chip8->memory[pos++] = (uint8_t)c;
  }
  if (c != EOF && pos > 0xFFF) {
    fprintf(stderr, "ROM too big to fit in memory\n");
    exit(1);
  }

  fclose(rom);

  /* bruh fseek and ftell can be used but as we we know the start and end of the
   files in chip8 cuz its not complex like nes or gameboy with bank switching
   and big memory space */
  beepInit();
}

#define SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK ((SDL_AudioDeviceID)0xFFFFFFFFu);

uint8_t *audio_buf;
uint32_t audio_len;
SDL_AudioSpec audioSpec;
SDL_AudioSpec *returnSpec;
SDL_AudioDeviceID devid;

void beepInit() {
  const char *path = "./sounds/beep.wav";
  returnSpec = SDL_LoadWAV(path, &audioSpec, &audio_buf, &audio_len);
  if (!returnSpec) {
    fprintf(stderr, "Could not load WAV: %s\n", SDL_GetError());
    return exit(1);
  }
  devid = SDL_OpenAudioDevice(NULL, 0, &audioSpec, returnSpec, 0);

  if (!devid) {
    fprintf(stderr, "Could not detect the device %s\n", SDL_GetError());
    exit(1);
  }
  SDL_PauseAudioDevice(devid, 0); // 0 to pause and non-zero to unpause;
}

void beep() {
  int res = SDL_QueueAudio(devid, audio_buf, audio_len);
  if (res == -1) {
    printf("SDL_QueueAudio failed: %s\n", SDL_GetError());
    exit(1);
  }
}

void cleanup_beep() {
  SDL_CloseAudioDevice(devid);
  SDL_FreeWAV(audio_buf);
}

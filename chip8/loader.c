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

  printf("starting to load the rom");
  int pos = 0x200; // programs start at 0x200
  int c;
  while ((c = fgetc(rom)) != EOF) {
    chip8->memory[pos++] = (uint8_t)c;
  }

  fclose(rom);

  /* bruh fseek and ftell can be used but as we we know the start and end of the
   files in chip8 cuz its not complex like nes or gameboy with bank switching
   and big memory space */
}

#define SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK ((SDL_AudioDeviceID)0xFFFFFFFFu);

void beep() {
  const char *path = "./sounds/beep.wav";
  uint8_t *audio_buf;
  uint32_t audio_len;
  SDL_AudioSpec audioSpec;
  SDL_AudioSpec *returnSpec =
      SDL_LoadWAV(path, &audioSpec, &audio_buf, &audio_len);
  if (!returnSpec) {
    fprintf(stderr, "Could not load WAV: %s\n", SDL_GetError());
    return exit(1);
  }
  SDL_AudioDeviceID devid =
      SDL_OpenAudioDevice(NULL, 0, &audioSpec, returnSpec, 0);

  if (!devid) {
    fprintf(stderr, "Could not detect the device %s\n", SDL_GetError());
    return exit(1);
  }
  int res = SDL_QueueAudio(devid, audio_buf, audio_len);
  if (res == -1) {
    printf("SDL_QueueAudio failed: %s\n", SDL_GetError());
    return exit(1);
  }
  SDL_PauseAudioDevice(devid, 0); // 0 to pause and non-zero to unpause;
  SDL_Delay(1000);
}

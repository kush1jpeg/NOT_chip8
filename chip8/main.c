#include "chip8.h"
#include "funcTable.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }
  if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
    fprintf(stderr, "SDL audio init failed: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow("NOT_chip8", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 64 * 20, // 640 *2
                       32 * 20,                         // 320 *2
                       SDL_WINDOW_SHOWN);

  if (!window) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }
  SDL_RenderSetScale(renderer, 20, 20);
  bool running = true;
  SDL_Event e; // to track the key pressed;

  // to init the chip8;
  Chip8 chip8;
  memset(&chip8, 0, sizeof(Chip8)); // <- sets memory, registers, etc. to 0
  chip8.pc = 0x200;

  /*
    8 frames -> 500hz/60fps;
    500hz is the appx clock;
    speed of the chip8, so we are mimicing that;  */
  int cycles_per_frame = 8;
  uint32_t lastTimer = SDL_GetTicks64();

  // to fill the fontset;
  chip8_init(&chip8);

  // loading the rom;
  chip8_load(&chip8, "../rom/testROM.ch8");

  while (running) {
    // sending the instructions while maintaining ~500hz speed.
    for (int i = 0; i < cycles_per_frame; i++) {
      chip8_cycle(&chip8);
    }
    if ((SDL_GetTicks64() - lastTimer) >=
        1000 / 60) { // ~16 ms as 1sec for 60fps
      if (chip8.delay_timer > 0) {
        chip8.delay_timer--;
      }

      if (chip8.sound_timer > 0) {
        // beep();
        chip8.sound_timer--;
      }
      lastTimer = SDL_GetTicks64();
    }
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
      } else if (e.type == SDL_KEYDOWN) {
        handle_key_event(&e, 1, chip8.key);
      } else if (e.type == SDL_KEYUP) {
        handle_key_event(&e, 0, chip8.key);
      }
    }

    if (chip8.draw_flag) {
      drawGraphics(renderer, &chip8);
      chip8.draw_flag = false; // reset until next draw opcode
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}

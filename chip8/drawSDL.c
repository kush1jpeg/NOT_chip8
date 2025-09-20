#include "chip8.h"
#include <stdbool.h>

void drawGraphics(SDL_Renderer *renderer, Chip8 *chip8) {

  // Clear screen black
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      if (chip8->gfx[x + y * 64]) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }
  }

  SDL_RenderPresent(renderer);
}

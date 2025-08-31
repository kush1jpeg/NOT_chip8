#include "chip8.h"
#include <stdbool.h>

void drawGraphics(SDL_Renderer *renderer, Chip8 *chip8) {

  // Clear screen black
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw one white pixel
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_RenderPresent(renderer);
}

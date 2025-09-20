#ifndef CHIP8_H
#define CHIP8_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uint8_t memory[4096]; // 4K memory
  uint8_t V[16];        // 16 registers V0..VF
  uint16_t I;           // Index register
  uint16_t pc;          // Program counter

  uint16_t op; // to store the opcode

  uint8_t stack[16]; // Stack (16 levels)
  uint8_t sp;        // Stack pointer

  uint8_t delay_timer; // Delay timer
  uint8_t sound_timer; // Sound timer

  uint8_t key[16];      // Hex keypad state
  uint8_t gfx[64 * 32]; // 1D array of 64×32 = 2048 bytes.
  bool draw_flag;       // for the draw trigger
} Chip8;

void chip8_init(Chip8 *chip8);
void drawGraphics(SDL_Renderer *renderer, Chip8 *chip8);
void chip8_load(Chip8 *chip8, const char *rom_path);
void chip8_cycle(Chip8 *chip8); // fetch, decode, execute
void beep();

#endif

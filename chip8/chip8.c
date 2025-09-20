#include "chip8.h"
#include "funcTable.h"
#include <stdint.h>
#include <stdlib.h>

void check_stack_bounds(Chip8 *chip, const char *context) {
  if (chip->sp > 16) { // Chip-8 stack is 16 levels deep
    fprintf(stderr, "[FATAL] Stack overflow in %s (SP=%d)\n", context,
            chip->sp);
    exit(1);
  }
  if (chip->sp < 0) { // should never happen if sp is uint8_t
    fprintf(stderr, "[FATAL] Stack underflow in %s (SP=%d)\n", context,
            chip->sp);
    exit(1);
  }
}

// === Function lookup table ===
opcode_func main_table[16] = {
    op_0xxx, // 0x0***
    op_1nnn, // 0x1*** JMP addr
    op_2nnn, // 0x2*** CALL addr (TODO)
    op_3xkk, // 0x3*** SE Vx, byte (TODO)
    op_4xkk, // 0x4***
    op_5xy0, // 0x5***
    op_6xkk, // 0x6***
    op_7xkk, // 0x7***
    op_8xy_, // 0x8***
    op_9xy0, // 0x9***
    op_Annn, // 0xA***
    op_Bnnn, // 0xB***
    op_Cxkk, // 0xC***
    op_Dxyn, // 0xD*** DRW
    op_Ex__, // 0xE*** skip on key
    op_fxxx  // 0xF***
};

void chip8_cycle(Chip8 *chip8) {

  if (chip8->pc >= 4096) {
    printf("PC out of bounds: %X\n", chip8->pc);
    exit(1);
  }

  uint16_t opcode =
      (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];

  printf("PC: %03X  OPCODE: %04X\n", chip8->pc, opcode);

  chip8->pc += 2;

  uint8_t high_nibble = (opcode & 0xF000) >> 12; // 0x6 type shit

  main_table[high_nibble](chip8, opcode);
}

// === 0x1nnn JP addr ===
void op_1nnn(Chip8 *chip, uint16_t opcode) {
  printf("[JP] PC jumping from %03X to %03X\n", chip->pc, opcode & 0x0FFF);
  chip->pc = opcode & 0x0FFF;
}

// === 0x2nnn CALL addr ===
void op_2nnn(Chip8 *chip, uint16_t opcode) {
  check_stack_bounds(chip, "CALL-before");
  chip->stack[chip->sp] = chip->pc;
  chip->sp++;
  chip->pc = opcode & 0x0FFF;
  check_stack_bounds(chip, "CALL-after");
}

// === 0x3xkk SE Vx, byte ===
void op_3xkk(Chip8 *chip, uint16_t opcode) {
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t kk = opcode & 0x00FF;
  if (chip->V[x] == kk)
    chip->pc += 2;
}

// === 0x4xkk SNE Vx, byte ===
void op_4xkk(Chip8 *chip, uint16_t opcode) {
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t kk = opcode & 0x00FF;
  if (chip->V[x] != kk)
    chip->pc += 2;
}

// === 0x5xy0 SE Vx, Vy ===
void op_5xy0(Chip8 *chip, uint16_t opcode) {
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  if (chip->V[x] == chip->V[y])
    chip->pc += 2;
}

// === 0x6xkk LD Vx, byte ===
void op_6xkk(Chip8 *chip, uint16_t opcode) {
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t kk = opcode & 0x00FF;
  chip->V[x] = kk;
}

// === 0x7xkk ADD Vx, byte ===
void op_7xkk(Chip8 *chip, uint16_t opcode) {
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t kk = opcode & 0x00FF;
  chip->V[x] += kk;
}

void op_0xxx(Chip8 *chip, uint16_t opcode) {
  switch (opcode & 0x00FF) {
  case 0xE0: // CLS
    memset(chip->gfx, 0, sizeof(chip->gfx));
    chip->draw_flag = true;
    break;
  case 0xEE: // RET
    check_stack_bounds(chip, "CALL-before");
    printf("[STACK BEFORE] SP=%d | ", chip->sp);
    if (chip->sp > 0) {
      chip->sp--;
      printf("[STACK] SP=%d | ", chip->sp);
    }
    chip->pc = chip->stack[chip->sp];
    check_stack_bounds(chip, "CALL-after");
    break;
  default:
    break;
  }
}

// === 0x8xy* arithmetic ===
void op_8xy_(Chip8 *chip, uint16_t opcode) {
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  uint8_t sub = opcode & 0x000F;

  switch (sub) {
  case 0x0:
    chip->V[x] = chip->V[y];
    break;
  case 0x1:
    chip->V[x] |= chip->V[y];
    break;
  case 0x2:
    chip->V[x] &= chip->V[y];
    break;
  case 0x3:
    chip->V[x] ^= chip->V[y];
    break;
  case 0x4: { // ADD with carry
    uint16_t sum = chip->V[x] + chip->V[y];
    chip->V[0xF] = (sum > 0xFF);
    chip->V[x] = sum & 0xFF;
    break;
  }
  case 0x5: { // SUB
    chip->V[0xF] = (chip->V[x] > chip->V[y]);
    chip->V[x] -= chip->V[y];
    break;
  }
  case 0x6: { // SHR
    chip->V[0xF] = chip->V[x] & 0x1;
    chip->V[x] >>= 1;
    break;
  }
  case 0x7: { // SUBN
    chip->V[0xF] = (chip->V[y] > chip->V[x]);
    chip->V[x] = chip->V[y] - chip->V[x];
    break;
  }
  case 0xE: { // SHL
    chip->V[0xF] = (chip->V[x] & 0x80) >> 7;
    chip->V[x] <<= 1;
    break;
  }
  }
}

// === 0x9xy0 SNE Vx, Vy ===
void op_9xy0(Chip8 *chip, uint16_t opcode) {
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  if (chip->V[x] != chip->V[y])
    chip->pc += 2;
}

// === 0xAnnn LD I, addr ===
void op_Annn(Chip8 *chip, uint16_t opcode) { chip->I = opcode & 0x0FFF; }

// === 0xBnnn JP V0, addr ===
void op_Bnnn(Chip8 *chip, uint16_t opcode) {
  chip->pc = (opcode & 0x0FFF) + chip->V[0];
}

// === 0xCxkk RND Vx, byte ===
void op_Cxkk(Chip8 *chip, uint16_t opcode) {
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t kk = opcode & 0x00FF;
  chip->V[x] = (rand() % 256) & kk;
}

// === 0xDxyn DRW Vx, Vy, nibble ===
void op_Dxyn(Chip8 *chip, uint16_t opcode) {
  uint8_t x = chip->V[(opcode & 0x0F00) >> 8];
  uint8_t y = chip->V[(opcode & 0x00F0) >> 4];
  uint8_t height = opcode & 0x000F;
  chip->V[0xF] = 0;

  for (int row = 0; row < height; row++) {
    uint8_t sprite = chip->memory[chip->I + row];
    for (int col = 0; col < 8; col++) {
      if (sprite & (0x80 >> col)) {
        int px = (x + col) % 64;
        int py = (y + row) % 32;
        int idx = px + (py * 64);

        if (chip->gfx[idx] == 1)
          chip->V[0xF] = 1;
        chip->gfx[idx] ^= 1;
      }
    }
  }
  chip->draw_flag = true;
}

// === 0xEx__ key input ===
void op_Ex__(Chip8 *chip, uint16_t opcode) {
  uint8_t x = (opcode & 0x0F00) >> 8;
  switch (opcode & 0x00FF) {
  case 0x9E:
    if (chip->key[chip->V[x]])
      chip->pc += 2;
    break;
  case 0xA1:
    if (!chip->key[chip->V[x]])
      chip->pc += 2;
    break;
  }
}

// === 0xFx__ timers, registers ===
void op_fxxx(Chip8 *chip, uint16_t opcode) {
  uint8_t x = (opcode & 0x0F00) >> 8;
  switch (opcode & 0x00FF) {
  case 0x07:
    chip->V[x] = chip->delay_timer;
    break;
  case 0x0A: {
    // Wait for key
    bool key_pressed = false;
    for (int i = 0; i < 16; i++) {
      if (chip->key[i]) {
        chip->V[x] = i;
        key_pressed = true;
      }
    }
    if (!key_pressed)
      chip->pc -= 2; // retry same opcode
    break;
  }
  case 0x15:
    chip->delay_timer = chip->V[x];
    break;
  case 0x18:
    chip->sound_timer = chip->V[x];
    break;
  case 0x1E:
    chip->I += chip->V[x];
    break;
  case 0x29:
    chip->I = chip->V[x] * 5;
    break;     // font sprites
  case 0x33: { // BCD
    chip->memory[chip->I] = chip->V[x] / 100;
    chip->memory[chip->I + 1] = (chip->V[x] / 10) % 10;
    chip->memory[chip->I + 2] = chip->V[x] % 10;
    break;
  }
  case 0x55:
    for (int i = 0; i <= x; i++)
      chip->memory[chip->I + i] = chip->V[i];
    break;
  case 0x65:
    for (int i = 0; i <= x; i++)
      chip->V[i] = chip->memory[chip->I + i];
    break;
  }
}

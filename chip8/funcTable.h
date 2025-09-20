#ifndef TABLE_H
#define TABLE_H

#include "chip8.h"
typedef void (*opcode_func)(Chip8 *chip, uint16_t opcode);

// All opcode handlers
void op_0xxx(Chip8 *chip, uint16_t opcode);
void op_1nnn(Chip8 *chip, uint16_t opcode);
void op_2nnn(Chip8 *chip, uint16_t opcode);
void op_3xkk(Chip8 *chip, uint16_t opcode);
void op_4xkk(Chip8 *chip, uint16_t opcode);
void op_5xy0(Chip8 *chip, uint16_t opcode);
void op_6xkk(Chip8 *chip, uint16_t opcode);
void op_7xkk(Chip8 *chip, uint16_t opcode);
void op_8xy_(Chip8 *chip, uint16_t opcode);
void op_9xy0(Chip8 *chip, uint16_t opcode);
void op_Annn(Chip8 *chip, uint16_t opcode);
void op_Bnnn(Chip8 *chip, uint16_t opcode);
void op_Cxkk(Chip8 *chip, uint16_t opcode);
void op_Dxyn(Chip8 *chip, uint16_t opcode);
void op_Ex__(Chip8 *chip, uint16_t opcode);
void op_fxxx(Chip8 *chip, uint16_t opcode);
void op_NULL(Chip8 *chip, uint16_t opcode);

extern opcode_func main_table[16];

void handle_key_event(const SDL_Event *e, int value, uint8_t *chip8_keys);

void check_stack_bounds(Chip8 *chip, const char *context);

#endif

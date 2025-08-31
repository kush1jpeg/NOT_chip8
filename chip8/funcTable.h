#ifndef TABLE_H
#define TABLE_H

#include "chip8.h"
typedef void (*opcode_func)(Chip8 *chip, uint16_t opcode);

void op_0xxx(Chip8 *chip, uint16_t opcode);
void op_1nnn(Chip8 *chip, uint16_t opcode);
void op_6xkk(Chip8 *chip, uint16_t opcode);
void op_8xy_(Chip8 *chip, uint16_t opcode);
void op_dxyk(Chip8 *chip, uint16_t opcode);
void op_fxxx(Chip8 *chip, uint16_t opcode);
void op_NULL(Chip8 *chip, uint16_t opcode);

#endif // !TABLE

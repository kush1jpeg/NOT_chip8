#include "chip8.h"
#include "funcTable.h"
#include <stdint.h>

// === Function lookup table ===
opcode_func main_table[16] = {
    op_0xxx, // 0x0***
    op_1nnn, // 0x1***
    op_NULL, // 0x2*** CALL addr (TODO)
    op_NULL, // 0x3*** SE Vx, byte (TODO)
    op_NULL, // 0x4***
    op_NULL, // 0x5***
    op_6xkk, // 0x6***
    op_NULL, // 0x7***
    op_8xy_, // 0x8***
    op_NULL, // 0x9***
    op_NULL, // 0xA***
    op_NULL, // 0xB***
    op_NULL, // 0xC***
    op_NULL, // 0xD*** DRW
    op_NULL, // 0xE*** skip on key
    op_fxxx  // 0xF***
};

void chip8_cycle(Chip8 *chip8) {
  uint16_t opcode =
      (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
  chip8->pc += 2;
}

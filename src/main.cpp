#include "gb.h"

int main(void) {
    Gameboy gb;
    gb.mem[0] = 0x03;
    // gb.fetch_instruction();
    gb.step();
    gb.registers.print_regs();
}
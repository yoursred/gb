#include <iostream>
#include <iomanip>

#include "include.h"
#include "cpu/cpu.h"

CPU::Registers::Registers(void) {
    af = 0x1B0; bc = 0x13; de = 0xD8;
    hl = 0x14d; sp = 0xFFFE; pc = 0x100;
}

void CPU::Registers::print_regs(std::ostream& output) {
    output << "AF: " << COUT_HEX_WORD_DS(af) << " ";
    output << "BC: " << COUT_HEX_WORD_DS(bc) << " ";
    output << "DE: " << COUT_HEX_WORD_DS(de) << std::endl;
    output << "HL: " << COUT_HEX_WORD_DS(hl) << " ";
    output << "SP: " << COUT_HEX_WORD_DS(sp) << " ";
    output << "PC: " << COUT_HEX_WORD_DS(pc) << std::endl;
}

void CPU::Registers::print_flags(std::ostream& output) {
    output << "z\tn\th\tc" << std::endl;
    output << get_flag(FLAG_Z) << " ";
    output << get_flag(FLAG_N) << " ";
    output << get_flag(FLAG_H) << " ";
    output << get_flag(FLAG_C) << std::endl;
}

void CPU::Registers::set_flag(byte flag) {
    af |= flag;
}

void CPU::Registers::unset_flag(byte flag) {
    af &= ~((word) flag);
}

void CPU::Registers::update_flag(byte flag, word value) {
    if (value) {
        af |= flag;
    }
    else {
        af &= ~(flag);
    }
}

void CPU::Registers::flip_flag(byte flag) {
    af ^= flag;
}

int CPU::Registers::get_flag(byte flag) {
    return !!(af & flag);
}

int CPU::Registers::get_cc(byte cc) {
    switch (cc) {
        case CC_Z:
            return get_flag(FLAG_Z);
        case CC_NZ:
            return !get_flag(FLAG_Z);
        case CC_C:
            return get_flag(FLAG_C);
        case CC_NC:
            return !get_flag(FLAG_C);
        default:
            return 0;
    }
}

void CPU::Registers::set_flags(const char *flagstr) {
    for (int i = 0; flagstr[i] != 0 && i < 4; i++){
        switch(flagstr[i]) {
            case '0': unset_flag(0x10 << (3 - i)); break;
            case '1': set_flag(0x10 << (3 - i)); break;
        }
    }
}
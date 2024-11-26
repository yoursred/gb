#include <iostream>
#include <iomanip>

#include "include.h"
#include "cpu/cpu.h"

CPU::Registers::Registers():
    a(*((byte*) &af + 1)), f(*((byte*) &af)),
    b(*((byte*) &bc + 1)), c(*((byte*) &bc)),
    d(*((byte*) &de + 1)), e(*((byte*) &de)),
    h(*((byte*) &hl + 1)), l(*((byte*) &hl)),
    sph(*((byte*) &sp + 1)), spl(*((byte*) &sp)),
    w(*((byte*) &wz + 1)), z(*((byte*) &wz))
{
    af = 0x1B0; bc = 0x13; de = 0xD8;
    hl = 0x14d; sp = 0xFFFE; pc = 0x100;
    wz = 0;
    // a = *((byte*) &af);
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
    output << "z n h c" << std::endl;
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
            case '~': flip_flag(0x10 << (3 - i)); break;
        }
    }
}

void CPU::Registers::set_flags(const char *flagstr, byte operation, byte op1, byte op2) {
    word result = 0;
    byte h = 0; 
    switch (operation) {
        case OP_ADD:
        case OP_ADD16:
        case OP_INC:
        result = op1 + op2;
        h = ((op1 & 0xF) + (op2 & 0xF)) & 0x10;
        break;
        case OP_DEC:
        case OP_SUB:
        result = op1 - op2;
        h = ((op1 & 0xF) - (op2 & 0xF)) & 0x10;
        break;
        case OP_ADC:
        result = op1 + op2 + get_flag(FLAG_C);
        h = ((op1 & 0xF) + (op2 & 0xF) + get_flag(FLAG_H)) & 0x10;
        break;
        case OP_SBC:
        result = op1 - op2;
        h = ((op1 & 0xF) - (op2 & 0xF) - get_flag(FLAG_C)) & 0x10;
        break;
        case OP_LGC:
        result = op1;
        break;
    }

    for (int i = 0; flagstr[i] != 0 && i < 4; i++){
        switch(flagstr[i]) {
            case '0':
            unset_flag(0x10 << (3 - i));
            break;
            case '1':
            set_flag(0x10 << (3 - i));
            break;
            case '~':
            flip_flag(0x10 << (3 - i));
            break;
            case 'z':
            if (operation & (OP_ARTM | OP_IDU | OP_LGC))
                update_flag(FLAG_Z, !!result);
            break;
            case 'h':
            if (operation & (OP_ARTM | OP_IDU | OP_ADD16))
                update_flag(FLAG_H, h);
            break;
            case 'c':
            if (operation & (OP_ARTM | OP_ADD16))
                update_flag(FLAG_C, result & 0x100);
            break;
        }
    }
}
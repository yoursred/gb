#include<iostream>
#include<stdio.h>

#include "include.h"
#include "cpu/cpu.h"

CPU::Registers::Registers(void) {
    af = NW; bc = NW; de = NW;
    hl = NW; sp = NW; pc = NW;

    f = (byte*) af; a = f + 1;
    c = (byte*) bc; b = c + 1;
    e = (byte*) de; d = e + 1;
    l = (byte*) hl; h = l + 1;
    // CPU::mem;
}

void CPU::Registers::print_regs(void) {
    printf("AF=%04x BC=%04x DE=%04x HL=%04x SP=%04x PC=%04x\n",
        *af, *bc, *de, *hl, *sp, *pc);
}

void CPU::Registers::print_flags(void) {
    printf("z=%d n=%d h=%d c=%d\n",
    get_flag(FLAG_Z), get_flag(FLAG_N), get_flag(FLAG_H), get_flag(FLAG_C));
}

void CPU::Registers::set_flag(byte flag) {
    *f |= flag;
}

void CPU::Registers::unset_flag(byte flag) {
    *f &= ~(flag);
}

void CPU::Registers::update_flag(byte flag, word value) {
    if (value) {
        *f |= flag;
    }
    else {
        *f &= ~(flag);
    }
}

void CPU::Registers::flip_flag(byte flag) {
    *f ^= flag;
}

int CPU::Registers::get_flag(byte flag) {
    return !!(*f & flag);
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
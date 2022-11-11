#include<iostream>
#include<stdio.h>

#include "gb.h"
#include "insset.h"


Gameboy::GbRegisters::GbRegisters(void) {
    af = NW; bc = NW; de = NW;
    hl = NW; sp = NW; pc = NW;

    f = (byte*) af; a = f + 1;
    c = (byte*) bc; b = c + 1;
    e = (byte*) de; d = e + 1;
    l = (byte*) hl; h = l + 1;
    // Gameboy::mem;
}

void Gameboy::GbRegisters::print_regs(void) {
    printf("AF=%04x BC=%04x DE=%04x\nHL=%04x SP=%04x PC=%04x\n",
        *af, *bc, *de, *hl, *sp, *pc);
}

void Gameboy::GbRegisters::print_flags(void) {
    printf("z=%d n=%d h=%d c=%d\n",
    get_flag(FLAG_Z), get_flag(FLAG_N), get_flag(FLAG_H), get_flag(FLAG_C));
}

void Gameboy::GbRegisters::set_flag(byte flag) {
    *f |= flag;
}

void Gameboy::GbRegisters::unset_flag(byte flag) {
    *f &= ~(flag);
}

void Gameboy::GbRegisters::update_flag(byte flag, word value) {
    if (value) {
        *f |= flag;
    }
    else {
        *f &= ~(flag);
    }
}

void Gameboy::GbRegisters::flip_flag(byte flag) {
    *f ^= flag;
}

int Gameboy::GbRegisters::get_flag(byte flag) {
    return !!(*f & flag);
}

int Gameboy::GbRegisters::get_cc(byte cc) {
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

void Gameboy::GbRegisters::set_flags(const char *flagstr, const byte *a, const byte *b, const byte result) {
    for (int i = 0; flagstr[i] != 0 && i < 4; i++){
        switch(flagstr[i]) {
            case '0': unset_flag(0x10 << (3 - i)); break;
            case '1': set_flag(0x10 << (3 - i)); break;
            case 'z': update_flag(FLAG_Z, result); break;
            case 'n': set_flag(FLAG_N); break;
            case 'h': update_flag(FLAG_H, ((*a & 0xf) + (*b & 0xf)) & 0x10); break;
            case 'c': update_flag(FLAG_C, (int)(*a + *b) & 0x100); break;
        }
    }
}



Gameboy::Gameboy() {
    Gameboy::registers = Gameboy::GbRegisters();
    // Gameboy::registers.mem = 

    // GbRegisters();
}

byte Gameboy::fetch_instruction() {
    Gameboy::opcode = mem[*registers.pc];
    byte length = get_length(Gameboy::opcode);
    printf("opc=0x%02X, length=%d\n", Gameboy::opcode, length);
    switch (length) { // maybe we don't really need this?
        case 3:
            working_word = mem[*registers.pc + 1];
            working_word = (word) mem[*registers.pc + 2] << 8;
            break;
        case 2:
            working_byte = mem[*registers.pc + 1];
            break;
    }
    printf("wb = 0x%02X, ww = 0x%04X\n", working_byte, working_word);
    return length;
}

void Gameboy::step() {
    Gameboy::new_pc += Gameboy::fetch_instruction();
    switch (Gameboy::opcode) {
        case (0x03): Gameboy::INC(registers.bc);break;
        case (0x13):break;
        case (0x23):break;
        case (0x43):break;
    }
}

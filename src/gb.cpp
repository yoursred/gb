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
    printf("AF=%04x BC=%04x DE=%04x HL=%04x SP=%04x PC=%04x\n",
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

void Gameboy::GbRegisters::set_flags(const char *flagstr) {
    for (int i = 0; flagstr[i] != 0 && i < 4; i++){
        switch(flagstr[i]) {
            case '0': unset_flag(0x10 << (3 - i)); break;
            case '1': set_flag(0x10 << (3 - i)); break;
        }
    }
}



Gameboy::Gameboy() {
    Gameboy::R = Gameboy::GbRegisters();
    instructions = 0;
    // Gameboy::registers.mem = 

    // GbRegisters();
}

byte* Gameboy::mem_at(word* address) {
    return &mem[*address];
}

byte* Gameboy::mem_at(word address) {
    return &mem[address];
}

byte Gameboy::fetch_instruction() {
    Gameboy::opcode = mem[*R.pc];
    byte length;
    if (opcode != 0xCB){
        length = get_length(Gameboy::opcode);
        Gameboy::current_cycles = get_cycles(Gameboy::opcode);
    } else {
        length = get_length_prefixed(Gameboy::opcode); // Wholly redundant, it's always =2
        Gameboy::current_cycles = get_cycles_prefixed(Gameboy::opcode);
    }
    // printf("opc=0x%02X, length=%d\n", Gameboy::opcode, length);
    switch (length) { // maybe we don't really need this?
        case 3:
            working_word = mem[*R.pc + 1];
            working_word |= (word) mem[*R.pc + 2] << 8;
            break;
        case 2:
            working_byte = mem[*R.pc + 1];
            break;
    }
    // printf("wb = 0x%02X, ww = 0x%04X\n", working_byte, working_word);
    return length;
}
void Gameboy::step() {
    new_pc += fetch_instruction();
    switch (opcode) {
        case (0x00): break;
        case (0xCB): decode_prefixed(); break;
        default: decode();
    }
    instructions  += 1;
    cycles += current_cycles;
    *R.pc = new_pc;
}
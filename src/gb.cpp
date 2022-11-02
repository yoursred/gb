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



Gameboy::Gameboy() {
    // Gameboy::registers = GbRegisters();
    GbRegisters();
}

void Gameboy::fetch_instruction() {
    Gameboy::opcode = mem[*registers.pc];
    byte length = get_length(Gameboy::opcode);
    printf("opc=0x%02x, length=%d\n", Gameboy::opcode, length);
}

// --SECTION-- ARITHMETIC
// template <typename T>
// void Gameboy::INC(Operand<T> dst) {
//     byte one = 1;
//     if (!(dst.type & OP_16)) {
//         registers.set_flags("z0h-", dst, Operand<byte>(0, &one));
//     }
//     *dst.ptr++;
// }
// template void Gameboy::INC<byte>(Operand<byte> dst);
// template void Gameboy::INC<word>(Operand<word> dst);


// void Gameboy::SUB(Operand<byte> dst) {
// 
// }

// void Gameboy::XOR(Operand<byte> src) { // dst = a 
//     *registers.a ^= *src.ptr;
//     registers.set_flags<byte, byte>("z000", src, src);
// }

// --SECTION-- BIT OPS

// --SECTION-- BIT SHIFTS

// --SECTION-- LOAD
// template <typename T, typename U>
// void LD(Operand<T> dst, Operand<U> src) {
//     // if (dst.type & OP_PTR) {
//     //     // This is to cover memory indexed operands ([a16] and [$FF00 + a8/r8])
//     //     dst.ptr = &mem[!(dst.type & OP_16) * 0xFF00 + *dst.ptr];
//     // }
//     // if (src.type & OP_PTR) {
//     //     src.ptr = &mem[!(src.type & OP_16) * 0xFF00 + *src.ptr];
//     // }
//     *dst.ptr = *src.ptr;
// }


// --SECTION-- JUMPS
// void JR(Operand<byte> offset) {
//     new_pc = *offset.ptr;
// }

// byte JRC(byte cc, Operand<byte> offset) {
//     if (registers.get_cc(cc)) {
//         new_pc = *offset.ptr;
//         return 12;
//     }
//     return 8;
// }

// --SECTION-- STACK

// --SECTION-- MISC

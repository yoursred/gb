#include "gb.h"


// --SECTION-- ARITHMETIC
void Gameboy::ADC(byte *src) {
    byte x = *src + registers.get_flag(FLAG_C); // TODO: 
    Gameboy::registers.set_flags("z0hc", Gameboy::registers.a, &x, *Gameboy::registers.a + x);
    *Gameboy::registers.a += x;
}

void Gameboy::ADD(byte *src) {    
    Gameboy::registers.set_flags("z0hc", Gameboy::registers.a, src, *Gameboy::registers.a + *src);
    *Gameboy::registers.a += *src;
}
void Gameboy::ADD(word *src) {
    Gameboy::registers.set_flags("-0hc", Gameboy::registers.h, ((byte*)src) + 1, 0);
    *Gameboy::registers.a += *src;
}
void Gameboy::ADD_SP(byte *src) { // ADD SP, e8
    Gameboy::registers.set_flags("00hc", (byte*)Gameboy::registers.sp, src, *Gameboy::registers.sp + *src);
    *Gameboy::registers.a += *src;
}





void Gameboy::INC(byte *dst) {
    (*dst)++;
}
void Gameboy::INC(word *dst) {
    (*dst)++;
}


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
void Gameboy::JP(word *offset) {
    Gameboy::new_pc = *offset;
}

// byte JRC(byte cc, Operand<byte> offset) {
//     if (registers.get_cc(cc)) {
//         new_pc = *offset.ptr;
//         return 12;
//     }
//     return 8;
// }

// --SECTION-- STACK

// --SECTION-- MISC

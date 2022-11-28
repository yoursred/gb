#include "cpu/cpu.h"


// --SECTION-- ARITHMETIC
void CPU::ADC(MP src) {
    byte x = src;
    x += R.get_flag(FLAG_C);
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, (((*R.a & 0xf) + (x & 0xf)) & 0x10));
    R.update_flag(FLAG_C, (((*R.a) + (x)) & 0x100));
    *CPU::R.a += x;
    R.update_flag(FLAG_Z, !(*R.a));
}

void CPU::ADD(MP src) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, (((*R.a & 0xf) + (src & 0xf)) & 0x10));
    R.update_flag(FLAG_C, (((*R.a) + (src)) & 0x100));
    *CPU::R.a += src;
    R.update_flag(FLAG_Z, !(*R.a));
}

void CPU::ADD(word *src) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, (((*R.a & 0xf00) + (*src & 0xf00)) & 0x1000));
    R.update_flag(FLAG_C, (((*R.a) + (*src)) & 0x10000));
    *CPU::R.a += *src;
}

void CPU::ADD_SP(sbyte *src) { // ADD SP, e8
    // TODO: flags, note: src is signed
    *CPU::R.a += *src;
    R.update_flag(FLAG_H, (((*R.sp & 0xf) + (*src & 0xf)) & 0x10));
    R.update_flag(FLAG_C, (((*R.sp & 0xff) + (*src)) & 0x100));
    R.set_flags("00--");

}

void CPU::AND(MP src){
    *R.a &= src;
    R.update_flag(FLAG_Z, !(*R.a));
    R.set_flags("-010");
}

void CPU::CP(MP src) {
    byte result = *R.a - src;
    R.update_flag(FLAG_Z, !(result));
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, (((*R.a & 0xf) - (src & 0xf)) & 0x10));
    R.update_flag(FLAG_C, *R.a < src);
}

void CPU::DEC(MP dst) {
    R.update_flag(FLAG_H, ((dst & 0xf) - 1) & 0x10);
    dst = dst - 1;
    R.update_flag(FLAG_Z, !(dst));
    R.set_flag(FLAG_N);
}

void CPU::DEC(word *dst) {
    (*dst)--;
}

void CPU::INC(MP dst) {
    R.update_flag(FLAG_H, ((dst & 0xf) + 1) & 0x10);
    dst = dst + 1;
    R.update_flag(FLAG_Z, !(dst));
    R.unset_flag(FLAG_N);
}

void CPU::INC(word *dst) {
    (*dst)++;
}

void CPU::OR(MP src) {
    *R.a |= src;
    R.update_flag(FLAG_Z, !(*R.a));
    R.set_flags("-000");
}

void CPU::SBC(MP src) {
    byte x = src + R.get_flag(FLAG_C);
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, (((*R.a & 0xf) - (x & 0xf)) & 0x10));
    R.update_flag(FLAG_C, *R.a < x);
    *CPU::R.a -= x;
    R.update_flag(FLAG_Z, !(*R.a));
}

void CPU::SUB(MP src) {
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, (((*R.a & 0xf) - (src & 0xf)) & 0x10));
    R.update_flag(FLAG_C, *R.a < src);
    *CPU::R.a -= src;
    R.update_flag(FLAG_Z, !(*R.a));
}

void CPU::XOR(MP src) {
    *R.a ^= src;
    R.update_flag(FLAG_Z, !(*R.a));
    R.set_flags("-000");
}


// --SECTION-- BIT OPS
void CPU::BIT(byte n, MP src) {
    R.update_flag(FLAG_Z, ~(src) & (1 << n)); //  Set if bit n of src is 0.
    R.set_flags("-01-");
}

void CPU::RES(byte n, MP dst) {
    dst = (dst & ~(1 << n));
}

void CPU::SET(byte n, MP dst) {
    dst = (dst | (1 << n));
}

void CPU::SWAP(MP dst) {
    byte x = dst;
    dst = ((x << 4) | (x >> 4));
    R.update_flag(FLAG_Z, !(x));
    R.set_flags("-000");
}

// --SECTION-- BIT SHIFTS
void CPU::RL(MP dst) {
    byte x = (dst << 1) | R.get_flag(FLAG_C);
    R.update_flag(FLAG_C, (dst) & 0x80);
    dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::RLC(MP dst) {
    byte x = (dst << 1) | !!((dst) & 0x80);
    R.update_flag(FLAG_C, (dst) & 0x80);
    dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::RR(MP dst) {
    byte x = (R.get_flag(FLAG_C) << 7) | (dst >> 1);
    R.update_flag(FLAG_C, (dst) & 0x1);
    dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::RRC(MP dst) {
    byte x =(dst << 7) | (dst >> 1);
    R.update_flag(FLAG_C, (dst) & 0x1);
    dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::SLA(MP dst) {
    R.update_flag(FLAG_C, (dst) & 0x80);
    dst = (dst << 1);
    R.update_flag(FLAG_Z, dst);
    R.set_flags("-00-");
}

void CPU::SRA(MP dst) {
    R.update_flag(FLAG_C, (dst) & 0x1);
    dst = (dst & 0x80) | (dst >> 1);
    R.update_flag(FLAG_Z, dst);
    R.set_flags("-00-");
}

void CPU::SRL(MP dst) {
    R.update_flag(FLAG_C, (dst) & 0x1);
    dst = (dst >> 1);
    R.update_flag(FLAG_Z, dst);
    R.set_flags("-00-");
}

// --SECTION-- LOAD
void CPU::LD(MP dst, MP src) {
    // NONWORKING
    dst = src;
}

void CPU::LD(word *dst, word *src) {
    *dst = *src;
}

void CPU::LD16SP(word dst) {
    memory[dst] = *R.sp & 0xFF;
    memory[dst + 1] = *R.sp >> 8;
}

void CPU::LDI(MP dst, MP src) {
    dst = src;
    (*R.hl)++;
}

void CPU::LDD(MP dst, MP src) {
    dst = src;
    (*R.hl)--;
}

void CPU::LDHL() {
    sbyte s = *((sbyte*) &working_byte);
    *R.hl = *R.sp + s;
    R.update_flag(FLAG_H, (((*R.sp & 0xf) + (s & 0xf)) & 0x10));
    R.update_flag(FLAG_C, (((*R.sp & 0xff) + (s)) & 0x100));
    R.set_flags("00--");
    
}


// --SECTION-- JUMPS
void CPU::CALL(word address) {
    memory[--(*R.sp)] = new_pc >> 8;
    memory[--(*R.sp)] = new_pc;
    new_pc = address;
}

void CPU::CALLC(byte cc, word address) {
    if (R.get_cc(cc)) {
        memory[--(*R.sp)] = new_pc >> 8;
        memory[--(*R.sp)] = new_pc;
        new_pc = address;
        current_cycles = 24;
    } else {
        current_cycles = 12;
    }
}

void CPU::JR(sbyte offset) {
    CPU::new_pc += offset;
}

void CPU::JRC(byte cc, sbyte offset) {
    if (R.get_cc(cc)) {
        CPU::new_pc += offset;
        current_cycles = 12;
    } else {
        current_cycles = 8;
    }
}

void CPU::JP(word address) {
    CPU::new_pc = address;
}

void CPU::JPHL(void) {
    CPU::new_pc = memory[R.hl];
    CPU::new_pc |= memory[*R.hl + 1] << 8;
}

void CPU::JPC(byte cc, word address) {
    if (R.get_cc(cc)) {
        CPU::new_pc = address;
        current_cycles = 16;
    } else {
        current_cycles = 12;
    }
}

void CPU::RET() {
    new_pc = memory[(*R.sp)++];
    new_pc |= memory[(*R.sp)++] << 8;
}

void CPU::RETC(byte cc) {
    if (R.get_cc(cc)) {
        new_pc = memory[(*R.sp)++];
        new_pc |= memory[(*R.sp)++] << 8;
        current_cycles = 20;
    } else {
        current_cycles = 8;
    }
}

void CPU::RETI() {
    new_pc = memory[(*R.sp)++];
    new_pc |= memory[(*R.sp)++] << 8;
    memory[0xFFFF] = 1;
}

void CPU::RST(byte vector) {
    new_pc = vector;
}


// --SECTION-- STACK
void CPU::PUSH(word *src) {
    memory[--(*R.sp)] = *src >> 8;
    memory[--(*R.sp)] = *src;
}

void CPU::POP(word *dst) {
    *dst = memory[(*R.sp)++];
    *dst |= memory[(*R.sp)++] << 8;
}

// --SECTION-- MISC
void CPU::CCF(void) {
    R.set_flags("-00-");
    R.flip_flag(FLAG_C);
}

void CPU::CPL(void) {
    *R.a = ~(*R.a);
    R.set_flags("-11-");
}

void CPU::DAA(void) {
    if (R.get_cc(CC_NZ)) {
        if (R.get_flag(FLAG_C) || *R.a > 0x99) {
            *R.a += 0x60; R.set_flag(FLAG_C);
        }
        if (R.get_flag(FLAG_H) || (*R.a & 0xf) > 0x9) {
            *R.a += 0x6;
        }
    }
    else {
        if (R.get_flag(FLAG_C)) {*R.a -= 0x60;}
        if (R.get_flag(FLAG_H)) {*R.a -= 0x6;}
    }
    R.update_flag(FLAG_Z, !(*R.a));
    R.unset_flag(FLAG_H);
}

void CPU::DI(void) {
    memory[0xFFFF] = 0;
}

void CPU::EI(void) {
    memory[0xFFFF] = 1;
}

void CPU::HALT(void) {
    is_halted = true;
}

void CPU::NOP(void) {;}

void CPU::SCF(void) {
    R.set_flags("-001");
}

void CPU::STOP(void) {
    ; // TODO: this other thing
}
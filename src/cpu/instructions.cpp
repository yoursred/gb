#include "cpu/cpu.h"


// --SECTION-- ARITHMETIC
void CPU::ADC(byte *src) {
    byte x = *src + R.get_flag(FLAG_C);
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf) + (x & 0xf) & 0x10));
    R.update_flag(FLAG_C, ((*R.a) + (x) & 0x100));
    *CPU::R.a += x;
    R.update_flag(FLAG_Z, !(*R.a));
}

void CPU::ADD(byte *src) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf) + (*src & 0xf) & 0x10));
    R.update_flag(FLAG_C, ((*R.a) + (*src) & 0x100));
    *CPU::R.a += *src;
    R.update_flag(FLAG_Z, !(*R.a));
}

void CPU::ADD(word *src) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf00) + (*src & 0xf00) & 0x1000));
    R.update_flag(FLAG_C, ((*R.a) + (*src) & 0x10000));
    *CPU::R.a += *src;
}

void CPU::ADD_SP(sbyte *src) { // ADD SP, e8
    // TODO: flags, note: src is signed
    *CPU::R.a += *src;
    R.update_flag(FLAG_H, ((*R.sp & 0xf) + (*src & 0xf) & 0x10));
    R.update_flag(FLAG_C, ((*R.sp & 0xff) + (*src) & 0x100));
    R.set_flags("00--");

}

void CPU::AND(byte *src){
    *R.a &= *src;
    R.update_flag(FLAG_Z, !(*R.a));
    R.set_flags("-010");
}

void CPU::CP(byte *src) {
    byte result = *R.a - *src;
    R.update_flag(FLAG_Z, !(result));
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf) - (*src & 0xf) & 0x10));
    R.update_flag(FLAG_C, *R.a < *src);
}

void CPU::DEC(byte *dst) {
    R.update_flag(FLAG_H, ((*dst & 0xf) - 1) & 0x10);
    (*dst)--;
    R.update_flag(FLAG_Z, !(*dst));
    R.set_flag(FLAG_N);
}

void CPU::DEC(word *dst) {
    (*dst)--;
}

void CPU::INC(byte *dst) {
    R.update_flag(FLAG_H, ((*dst & 0xf) + 1) & 0x10);
    (*dst)++;
    R.update_flag(FLAG_Z, !(*dst));
    R.unset_flag(FLAG_N);
}

void CPU::INC(word *dst) {
    (*dst)++;
}

void CPU::OR(byte *src) {
    *R.a |= *src;
    R.update_flag(FLAG_Z, !(*R.a));
    R.set_flags("-000");
}

void CPU::SBC(byte *src) {
    byte x = *src + R.get_flag(FLAG_C);
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf) - (x & 0xf) & 0x10));
    R.update_flag(FLAG_C, *R.a < x);
    *CPU::R.a -= x;
    R.update_flag(FLAG_Z, !(*R.a));
}

void CPU::SUB(byte *src) {
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf) - (*src & 0xf) & 0x10));
    R.update_flag(FLAG_C, *R.a < *src);
    *CPU::R.a -= *src;
    R.update_flag(FLAG_Z, !(*R.a));
}

void CPU::XOR(byte *src) {
    *R.a ^= *src;
    R.update_flag(FLAG_Z, !(*R.a));
    R.set_flags("-000");
}


// --SECTION-- BIT OPS
void CPU::BIT(byte n, byte *src) {
    R.update_flag(FLAG_Z, ~(*src) & (1 << n)); //  Set if bit n of src is 0.
    R.set_flags("-01-");
}

void CPU::RES(byte n, byte *dst) {
    (*dst) &= ~(1 << n);
}

void CPU::SET(byte n, byte *dst) {
    (*dst) |= (1 << n);
}

void CPU::SWAP(byte *dst) {
    byte x = *dst;
    *dst = ((x << 4) | (x >> 4));
    R.update_flag(FLAG_Z, !(x));
    R.set_flags("-000");
}

// --SECTION-- BIT SHIFTS
void CPU::RL(byte *dst) {
    byte x = (*dst << 1) | R.get_flag(FLAG_C);
    R.update_flag(FLAG_C, (*dst) & 0x80);
    *dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::RLC(byte *dst) {
    byte x = (*dst << 1) | !!((*dst) & 0x80);
    R.update_flag(FLAG_C, (*dst) & 0x80);
    *dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::RR(byte *dst) {
    byte x = (R.get_flag(FLAG_C) << 7) | (*dst >> 1);
    R.update_flag(FLAG_C, (*dst) & 0x1);
    *dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::RRC(byte *dst) {
    byte x =(*dst << 7) | (*dst >> 1);
    R.update_flag(FLAG_C, (*dst) & 0x1);
    *dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::SLA(byte *dst) {
    R.update_flag(FLAG_C, (*dst) & 0x80);
    (*dst) <<= 1;
    R.update_flag(FLAG_Z, *dst);
    R.set_flags("-00-");
}

void CPU::SRA(byte *dst) {
    R.update_flag(FLAG_C, (*dst) & 0x1);
    *dst = (*dst & 0x80) | (*dst >> 1);
    R.update_flag(FLAG_Z, *dst);
    R.set_flags("-00-");
}

void CPU::SRL(byte *dst) {
    R.update_flag(FLAG_C, (*dst) & 0x1);
    (*dst) >>= 1;
    R.update_flag(FLAG_Z, *dst);
    R.set_flags("-00-");
}

// --SECTION-- LOAD
void CPU::LD(byte *dst, byte *src) {
    *dst = *src;
}

void CPU::LD(word *dst, word *src) {
    *dst = *src;
}

void CPU::LD16SP(word dst) {
    *mem_at(dst) = *R.sp & 0xFF;
    *mem_at(dst + 1) = *R.sp >> 8;
}

void CPU::LDI(byte *dst, byte *src) {
    *dst = *src;
    (*R.hl)++;
}

void CPU::LDD(byte *dst, byte *src) {
    *dst = *src;
    (*R.hl)--;
}

void CPU::LDHL() {
    sbyte s = *((sbyte*) &working_byte);
    *R.hl = *R.sp + s;
    R.update_flag(FLAG_H, ((*R.sp & 0xf) + (s & 0xf) & 0x10));
    R.update_flag(FLAG_C, ((*R.sp & 0xff) + (s) & 0x100));
    R.set_flags("00--");
    
}


// --SECTION-- JUMPS
void CPU::CALL(word *address) {
    mem[--(*R.sp)] = new_pc >> 8;
    mem[--(*R.sp)] = new_pc;
    new_pc = *address;
}

void CPU::CALLC(byte cc, word *address) {
    if (R.get_cc(cc)) {
        mem[--(*R.sp)] = new_pc >> 8;
        mem[--(*R.sp)] = new_pc;
        new_pc = *address;
        current_cycles = 24;
    } else {
        current_cycles = 12;
    }
}

void CPU::JR(sbyte *offset) {
    CPU::new_pc += *offset;
}

void CPU::JRC(byte cc, sbyte *offset) {
    if (R.get_cc(cc)) {
        CPU::new_pc += *offset;
        current_cycles = 12;
    } else {
        current_cycles = 8;
    }
}

void CPU::JP(word *address) {
    CPU::new_pc = *address;
}

void CPU::JPC(byte cc, word *address) {
    if (R.get_cc(cc)) {
        CPU::new_pc = *address;
        current_cycles = 16;
    } else {
        current_cycles = 12;
    }
}

void CPU::RET() {
    new_pc = mem[(*R.sp)++];
    new_pc |= mem[(*R.sp)++] << 8;
}

void CPU::RETC(byte cc) {
    if (R.get_cc(cc)) {
        new_pc = mem[(*R.sp)++];
        new_pc |= mem[(*R.sp)++] << 8;
        current_cycles = 20;
    } else {
        current_cycles = 8;
    }
}

void CPU::RETI() {
    new_pc = mem[(*R.sp)++];
    new_pc |= mem[(*R.sp)++] << 8;
    mem[0xFFFF] = 1;
}

void CPU::RST(byte vector) {
    new_pc = vector;
}


// --SECTION-- STACK
void CPU::PUSH(word *src) {
    mem[--(*R.sp)] = *src >> 8;
    mem[--(*R.sp)] = *src;
}

void CPU::POP(word *dst) {
    *dst = mem[(*R.sp)++];
    *dst |= mem[(*R.sp)++] << 8;
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
    mem[0xFFFF] = 0;
}

void CPU::EI(void) {
    mem[0xFFFF] = 1;
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
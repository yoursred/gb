#include "gb.h"


// --SECTION-- ARITHMETIC
void Gameboy::ADC(byte *src) {
    byte x = *src + R.get_flag(FLAG_C);
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf) + (x & 0xf) & 0x10));
    R.update_flag(FLAG_C, ((*R.a) + (x) & 0x100));
    *Gameboy::R.a += x;
    R.update_flag(FLAG_Z, !(*R.a));
}

void Gameboy::ADD(byte *src) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf) + (*src & 0xf) & 0x10));
    R.update_flag(FLAG_C, ((*R.a) + (*src) & 0x100));
    *Gameboy::R.a += *src;
    R.update_flag(FLAG_Z, !(*R.a));
}

void Gameboy::ADD(word *src) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf00) + (*src & 0xf00) & 0x1000));
    R.update_flag(FLAG_C, ((*R.a) + (*src) & 0x10000));
    *Gameboy::R.a += *src;
}

void Gameboy::ADD_SP(byte *src) { // ADD SP, e8
    // TODO: flags, note: src is signed
    *Gameboy::R.a += *src;
}

void Gameboy::AND(byte *src){
    *R.a &= *src;
    R.update_flag(FLAG_Z, !(*R.a));
    R.set_flags("-010");
}

void Gameboy::CP(byte *src) {
    byte result = *R.a - *src;
    R.update_flag(FLAG_Z, !(result));
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf) - (*src & 0xf) & 0x10));
    R.update_flag(FLAG_C, *R.a < *src);
}

void Gameboy::DEC(byte *dst) {
    R.update_flag(FLAG_H, ((*dst & 0xf) - 1) & 0x10);
    (*dst)--;
    R.update_flag(FLAG_Z, !(*dst));
    R.set_flag(FLAG_N);
}

void Gameboy::DEC(word *dst) {
    (*dst)--;
}

void Gameboy::INC(byte *dst) {
    R.update_flag(FLAG_H, ((*dst & 0xf) + 1) & 0x10);
    (*dst)++;
    R.update_flag(FLAG_Z, !(*dst));
    R.unset_flag(FLAG_N);
}

void Gameboy::INC(word *dst) {
    (*dst)++;
}

void Gameboy::OR(byte *src) {
    *R.a |= *src;
    R.update_flag(FLAG_Z, !(*R.a));
    R.set_flags("-000");
}

void Gameboy::SBC(byte *src) {
    byte x = *src + R.get_flag(FLAG_C);
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf) - (x & 0xf) & 0x10));
    R.update_flag(FLAG_C, *R.a < x);
    *Gameboy::R.a -= x;
    R.update_flag(FLAG_Z, !(*R.a));
}

void Gameboy::SUB(byte *src) {
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, ((*R.a & 0xf) - (*src & 0xf) & 0x10));
    R.update_flag(FLAG_C, *R.a < *src);
    *Gameboy::R.a -= *src;
    R.update_flag(FLAG_Z, !(*R.a));
}

void Gameboy::XOR(byte *src) {
    *R.a ^= *src;
    R.update_flag(FLAG_Z, !(*R.a));
    R.set_flags("-000");
}


// --SECTION-- BIT OPS
void Gameboy::BIT(byte bit, byte *src) {
    R.update_flag(FLAG_Z, (*src) & (1 << bit));
    R.set_flags("-01-");
}

void Gameboy::RES(byte bit, byte *dst) {
    (*dst) &= ~(1 << bit);
}

void Gameboy::SET(byte bit, byte *dst) {
    (*dst) |= (1 << bit);
}

void Gameboy::SWAP(byte *dst) {
    byte x = *dst;
    *dst = ((x << 4) | (x >> 4));
    R.update_flag(FLAG_Z, !(x));
    R.set_flags("-000");
}

// --SECTION-- BIT SHIFTS
void Gameboy::RL(byte *dst) {
    byte x = (*dst << 1) | R.get_flag(FLAG_C);
    R.update_flag(FLAG_C, (*dst) & 0x80);
    *dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void Gameboy::RLC(byte *dst) {
    byte x = (*dst << 1) | !!((*dst) & 0x80);
    R.update_flag(FLAG_C, (*dst) & 0x80);
    *dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void Gameboy::RR(byte *dst) {
    byte x = (R.get_flag(FLAG_C) << 7) | (*dst >> 1);
    R.update_flag(FLAG_C, (*dst) & 0x1);
    *dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void Gameboy::RRC(byte *dst) {
    byte x =(*dst << 7) | (*dst >> 1);
    R.update_flag(FLAG_C, (*dst) & 0x1);
    *dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void Gameboy::SLA(byte *dst) {
    R.update_flag(FLAG_C, (*dst) & 0x80);
    (*dst) <<= 1;
    R.update_flag(FLAG_Z, *dst);
    R.set_flags("-00-");
}

void Gameboy::SRA(byte *dst) {
    R.update_flag(FLAG_C, (*dst) & 0x1);
    *dst = (*dst & 0x80) | (*dst >> 1);
    R.update_flag(FLAG_Z, *dst);
    R.set_flags("-00-");
}

void Gameboy::SRL(byte *dst) {
    R.update_flag(FLAG_C, (*dst) & 0x1);
    (*dst) >>= 1;
    R.update_flag(FLAG_Z, *dst);
    R.set_flags("-00-");
}

// --SECTION-- LOAD
void Gameboy::LD(byte *dst, byte *src) {
    *dst = *src;
}

void Gameboy::LD(word *dst, word *src) {
    *dst = *src;
}

void Gameboy::LDI(byte *dst, byte *src) {
    *dst = *src;
    (*R.hl)++;
}

void Gameboy::LDD(byte *dst, byte *src) {
    *dst = *src;
    (*R.hl)--;
}

void Gameboy::LDHL() {
    *R.hl = *R.sp + working_byte;
}


// --SECTION-- JUMPS
void Gameboy::CALL(word *address) {
    mem[--(*R.sp)] = new_pc >> 8;
    mem[--(*R.sp)] = new_pc;
    new_pc = *address;
}

void Gameboy::CALLC(byte cc, word *address) {
    if (R.get_cc(cc)) {
        mem[--(*R.sp)] = new_pc >> 8;
        mem[--(*R.sp)] = new_pc;
        new_pc = *address;
        current_cycles = 24;
    } else {
        current_cycles = 12;
    }
}

void Gameboy::JR(byte *offset) {
    Gameboy::new_pc += *offset;
}

void Gameboy::JRC(byte cc, byte *offset) {
    if (R.get_cc(cc)) {
        Gameboy::new_pc += *offset;
        current_cycles = 12;
    } else {
        current_cycles = 8;
    }
}

void Gameboy::JP(word *address) {
    Gameboy::new_pc = *address;
}

void Gameboy::JPC(byte cc, word *address) {
    if (R.get_cc(cc)) {
        Gameboy::new_pc = *address;
        current_cycles = 16;
    } else {
        current_cycles = 12;
    }
}

void Gameboy::RET() {
    new_pc = mem[(*R.sp)++];
    new_pc |= mem[(*R.sp)++] << 8;
}

void Gameboy::RETC(byte cc) {
    if (R.get_cc(cc)) {
        new_pc = mem[(*R.sp)++];
        new_pc |= mem[(*R.sp)++] << 8;
        current_cycles = 20;
    } else {
        current_cycles = 8;
    }
}

void Gameboy::RETI() {
    new_pc = mem[(*R.sp)++];
    new_pc |= mem[(*R.sp)++] << 8;
    mem[0xFFFF] = 1;
}

void Gameboy::RST(byte vector) {
    new_pc = vector;
}


// --SECTION-- STACK
void Gameboy::PUSH(word *src) {
    mem[--(*R.sp)] = *src >> 8;
    mem[--(*R.sp)] = *src;
}

void Gameboy::POP(word *dst) {
    *dst = mem[(*R.sp)++];
    *dst |= mem[(*R.sp)++] << 8;
}

// --SECTION-- MISC
void Gameboy::CCF(void) {
    R.set_flags("-00-");
    R.flip_flag(FLAG_C);
}

void Gameboy::CPL(void) {
    *R.a = ~(*R.a);
    R.set_flags("-11-");
}

void Gameboy::DAA(void) {
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

void Gameboy::DI(void) {
    mem[0xFFFF] = 0;
}

void Gameboy::EI(void) {
    mem[0xFFFF] = 1;
}

void Gameboy::HALT(void) {
    ; // TODO: this thing
}

void Gameboy::NOP(void) {;}

void Gameboy::SCF(void) {
    R.set_flags("-001");
}

void Gameboy::STOP(void) {
    ; // TODO: this other thing
}
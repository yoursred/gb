#include "cpu/cpu.h"


// --SECTION-- ARITHMETIC
void CPU::ADC(MP src) {
    byte x = src;
    word result = R_A + R.get_flag(FLAG_C) + x;
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, (((R_A & 0xf) + (x & 0xf) + R.get_flag(FLAG_C)) & 0x10));
    R.update_flag(FLAG_C, result > 0xff);
    R_A = result;
    R.update_flag(FLAG_Z, !(result & 0xFF));
}

void CPU::ADD(MP src) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, (((R_A & 0xf) + (src & 0xf)) & 0x10));
    R.update_flag(FLAG_C, (((R_A) + (src)) & 0x100));
    R_A += src;
    R.update_flag(FLAG_Z, !(R_A));
}

void CPU::ADD(word& src) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, (((R.hl & 0xFFF) + (src & 0xFFF)) & 0x1000));
    R.update_flag(FLAG_C, ((((unsigned int) R.hl) + ((unsigned int) src)) > 0xFFFF));
    R.hl += src;
}

void CPU::ADD_SP(sbyte src) { // ADD SP, e8
    // TODO: flags, note: src is signed
    sword offset = src;
    R.update_flag(FLAG_H, (((R.sp & 0xf) + (offset & 0xf)) > 0xf));
    R.update_flag(FLAG_C, (((R.sp & 0xff) + (offset & 0xFF)) > 0xFF));
    R.sp += offset;
    R.set_flags("00--");

}

void CPU::AND(MP src){
    R_A &= src;
    // MP(R.af, R_HI) = (MP(R.af, R_HI) & src);
    R.update_flag(FLAG_Z, !(R_A));
    R.set_flags("-010");
}

void CPU::CP(MP src) {
    byte result = R_A - src;
    R.update_flag(FLAG_Z, !(result));
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, (((R_A & 0xf) - (src & 0xf)) & 0x10));
    R.update_flag(FLAG_C, R_A < src);
}

void CPU::DEC(MP dst) {
    R.update_flag(FLAG_H, ((dst & 0xf) - 1) & 0x10);
    dst = dst - 1;
    R.update_flag(FLAG_Z, !(dst));
    R.set_flag(FLAG_N);
}

void CPU::DEC(word& dst) {
    dst--;
}

void CPU::INC(MP dst) {
    R.update_flag(FLAG_H, ((dst & 0xf) + 1) & 0x10);
    dst = dst + 1;
    R.update_flag(FLAG_Z, !(dst));
    R.unset_flag(FLAG_N);
}

void CPU::INC(word& dst) {
    dst++;
}

void CPU::OR(MP src) {
    R_A |= src;
    R.update_flag(FLAG_Z, !(R_A));
    R.set_flags("-000");
}

void CPU::SBC(MP src) {
    byte x = src;
    byte result = R_A - R.get_flag(FLAG_C) - x;
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, ((R_A & 0xf) < (x & 0xf) + R.get_flag(FLAG_C)));
    R.update_flag(FLAG_C, R_A < (x + R.get_flag(FLAG_C)));
    R_A = result;
    R.update_flag(FLAG_Z, !(result));
}

void CPU::SUB(MP src) {
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, (((R_A & 0xf) - (src & 0xf)) & 0x10));
    R.update_flag(FLAG_C, R_A < src);
    R_A -= src;
    R.update_flag(FLAG_Z, !(R_A));
}

void CPU::XOR(MP src) {
    R_A ^= src;
    R.update_flag(FLAG_Z, !(R_A));
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
    R.update_flag(FLAG_C, ((byte) dst) & 0x80);
    dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::RLC(MP dst) {
    byte x = (dst << 1) | !!((dst) & 0x80);
    R.update_flag(FLAG_C, ((byte) dst) & 0x80);
    dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::RR(MP dst) {
    byte x = (R.get_flag(FLAG_C) << 7) | (dst >> 1);
    R.update_flag(FLAG_C, ((byte) dst) & 0x1);
    dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::RRC(MP dst) {
    byte x =(dst << 7) | (dst >> 1);
    R.update_flag(FLAG_C, ((byte) dst) & 0x1);
    dst = x;
    R.update_flag(FLAG_Z, !x);
    R.set_flags("-00-");
}

void CPU::SLA(MP dst) {
    R.update_flag(FLAG_C, ((byte) dst) & 0x80);
    dst = (dst << 1);
    R.update_flag(FLAG_Z, !dst);
    R.set_flags("-00-");
}

void CPU::SRA(MP dst) {
    R.update_flag(FLAG_C, ((byte) dst) & 0x1);
    dst = (dst & 0x80) | (dst >> 1);
    R.update_flag(FLAG_Z, !dst);
    R.set_flags("-00-");
}

void CPU::SRL(MP dst) {
    R.update_flag(FLAG_C, ((byte) dst) & 0x1);
    dst = (dst >> 1);
    R.update_flag(FLAG_Z, !dst);
    R.set_flags("-00-");
}

// --SECTION-- LOAD
void CPU::LD(MP dst, MP src) {
    dst = src;
}

void CPU::LD(word& dst, word& src) {
    dst = src;
}

void CPU::LD16SP(word dst) {
    memory[dst] = R.sp & 0xFF;
    memory[dst + 1] = R.sp >> 8;
    #ifdef __GHOST_DEBUG
    memory_writes += 2;
    #endif
}

void CPU::LDI(MP dst, MP src) {
    dst = src;
    R.hl++;
}

void CPU::LDD(MP dst, MP src) {
    dst = src;
    R.hl--;
}

void CPU::LDHL() {
    sword offset = (sbyte) working_byte;
    R.update_flag(FLAG_H, (((R.sp & 0xf) + (offset & 0xf)) > 0xF));
    R.update_flag(FLAG_C, (((R.sp & 0xff) + (offset & 0xFF)) > 0xFF));
    R.hl = R.sp + offset;
    R.set_flags("00--");
    
}


// --SECTION-- JUMPS
void CPU::CALL(word address) {
    memory[--(R.sp)] = new_pc >> 8;
    memory[--(R.sp)] = new_pc;
    #ifdef __GHOST_DEBUG
    memory_writes += 2;
    #endif
    new_pc = address;
}

void CPU::CALLC(byte cc, word address) {
    if (R.get_cc(cc)) {
        memory[--(R.sp)] = new_pc >> 8;
        memory[--(R.sp)] = new_pc;
        #ifdef __GHOST_DEBUG
        memory_writes += 2;
        #endif
        new_pc = address;
        current_cycles += 24;
    } else {
        current_cycles += 12;
    }
}

void CPU::JR(sbyte offset) {
    CPU::new_pc += offset;
}

void CPU::JRC(byte cc, sbyte offset) {
    if (R.get_cc(cc)) {
        CPU::new_pc += offset;
        current_cycles += 12;
    } else {
        current_cycles += 8;
    }
}

void CPU::JP(word address) {
    CPU::new_pc = address;
}

void CPU::JPHL(void) {
    // TODO: deprecated
    CPU::new_pc = memory[R.hl];
    CPU::new_pc |= memory[R.hl + 1] << 8;
}

void CPU::JPC(byte cc, word address) {
    if (R.get_cc(cc)) {
        CPU::new_pc = address;
        current_cycles += 16;
    } else {
        current_cycles += 12;
    }
}

void CPU::RET() {
    new_pc = memory[(R.sp)++];
    new_pc |= memory[(R.sp)++] << 8;
}

void CPU::RETC(byte cc) {
    if (R.get_cc(cc)) {
        new_pc = memory[(R.sp)++];
        new_pc |= memory[(R.sp)++] << 8;
        current_cycles += 20;
    } else {
        current_cycles += 8;
    }
}

void CPU::RETI() {
    new_pc = memory[(R.sp)++];
    new_pc |= memory[(R.sp)++] << 8;
    ime = true;
}

void CPU::RST(byte vector) {
    memory[--(R.sp)] = new_pc >> 8;
    memory[--(R.sp)] = new_pc;
    #ifdef __GHOST_DEBUG
    memory_writes += 2;
    #endif
    new_pc = vector;
    R.pc = vector;
}


// --SECTION-- STACK
void CPU::PUSH(word& src) {
    memory[--(R.sp)] = src >> 8;
    memory[--(R.sp)] = src;
    #ifdef __GHOST_DEBUG
    memory_writes += 2;
    #endif
}

void CPU::POP(word& dst) {
    dst = memory[(R.sp)++];
    dst |= memory[(R.sp)++] << 8;
    R.af &= 0xFFF0; // There is no war in Ba Sing Se
}

// --SECTION-- MISC
void CPU::CCF(void) {
    R.set_flags("-00-");
    R.flip_flag(FLAG_C);
}

void CPU::CPL(void) {
    R_A = ~(R_A);
    R.set_flags("-11-");
}

void CPU::DAA(void) {
    word result = R.af >> 8;
    R.unset_flag(FLAG_Z);
    if (R.get_flag(FLAG_N)) {
        if (R.get_flag(FLAG_H)) {
            result = (result - 0x06) & 0xFF;
        }
        if (R.get_flag(FLAG_C)) {
            result -= 0x60;
        }
    }
    else {
        if (R.get_flag(FLAG_H) || (result & 0xf) > 0x9) {
            result += 0x6;
        }
        if (R.get_flag(FLAG_C) || result > 0x9f) {
            result += 0x60;
        }
    }

    if ((result & 0xff) == 0) {
        R.set_flag(FLAG_Z);
    }
    if ((result & 0x100) == 0x100) {
        R.set_flag(FLAG_C);
    }
    R.unset_flag(FLAG_H);
    R.af &= 0xF0;
    R.af |= result << 8;
}

void CPU::DI(void) {
    ime = false;
}

void CPU::EI(void) {
    ime_buffer = EI_0;
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
    // Fuck this bs
}
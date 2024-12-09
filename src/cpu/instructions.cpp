#include <iostream>

#include "cpu/cpu.h"


// --SECTION-- 8-BIT LOADS
void CPU::LD(byte& r8, byte r8p) {
    r8 = r8p;
}
void CPU::LD(byte& r8) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        r8 = R.z;
        break;
    }
}

void CPU::LD(byte& r8, word r16ptr) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(r16ptr);
        break;
        case 1:
        r8 = R.z;
        break;
    }
}
void CPU::LD(word r16ptr, byte r8) {
    switch (tcycles >> 2) {
        case 0:
        memory.write(r16ptr, r8);
        break;
    }
}

void CPU::LD_HLptr_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        memory.write(R.hl, R.z);
        break;
    }
}

void CPU::LD_A_d16ptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.w = memory.read(R.pc++);
        break;
        case 2:
        R.z = memory.read(R.wz);
        break;
        case 3:
        R.a = R.z;
        break;
    }
}
void CPU::LD_d16ptr_A() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.w = memory.read(R.pc++);
        break;
        case 2:
        memory.write(R.wz, R.a);
        break;
    }
}

void CPU::LDH_A_Cptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(0xFF00 | R.c);
        break;
        case 1:
        R.a = R.z;
        break;
    }
}
void CPU::LDH_Cptr_A() {
    switch (tcycles >> 2) {
        case 0:
        memory.write(0xFF00 | R.c, R.a);
        break;
    }
}

void CPU::LDH_A_d8ptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.z = memory.read(0xFF00 | R.z);
        break;
        case 2:
        R.a = R.z;
        break;
    }
}

void CPU::LDH_d8ptr_A() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        memory.write(0xFF00 | R.z, R.a);
    }
}

void CPU::LD_A_HLdptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl--);
        break;
        case 1:
        R.a = R.z;
        break;
    }
}

void CPU::LD_HLdptr_A() {
    switch (tcycles >> 2) {
        case 0:
        memory.write(R.hl--, R.a);
        break;
    }
}

void CPU::LD_A_HLiptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl++);
        break;
        case 1:
        R.a = R.z;
        break;
    }
}

void CPU::LD_HLiptr_A() {
    switch (tcycles >> 2) {
        case 0:
        memory.write(R.hl++, R.a);
        break;
    }
}

// --SECTION--16-BIT LOADS
void CPU::LD(word& r16) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.w = memory.read(R.pc++);
        break;
        case 2:
        r16 = R.wz;
        break;
    }
}

void CPU::LD_d16ptr_SP() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.w = memory.read(R.pc++);
        break;
        case 2:
        memory.write(R.wz++, R.spl);
        break;
        case 3:
        memory.write(R.wz, R.sph);
        break;
    }
}

void CPU::LD_SP_HL() {
    switch (tcycles >> 2) {
        case 0:
        R.sp = R.hl;
        break;
    }
}

void CPU::PUSH(word r16) {
    switch (tcycles >> 2) {
        case 0:
        R.sp--;
        break;
        case 1:
        memory.write(R.sp--, (r16 >> 8));
        break;
        case 2:
        memory.write(R.sp, r16 & 0xFF);
        break;
    }
}

void CPU::POP(word& r16) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.sp++);
        break;
        case 1:
        R.w = memory.read(R.sp++);
        break;
        case 2:
        r16 = R.wz;
        break;
    }
}

void CPU::POP_AF() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.sp++);
        break;
        case 1:
        R.w = memory.read(R.sp++);
        break;
        case 2:
        R.af = R.wz & 0xFFF0;
        break;
    }
}

void CPU::LD_HL_SP() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.set_flags("00hc", OP_ADD, R.z, R.spl);
        R.l = R.z + R.spl;
        break;
        case 2:
        R.h = R.sph + R.get_flag(FLAG_C) + (R.z >> 7) * 0xFF;
        break;
    }
}

 // --SECTION-- 8-BIT ARITHMETIC
void CPU::ADD(byte r8) {
    R.set_flags("z0hc", OP_ADD, R.a, r8);
    R.a = R.a + r8;
}

void CPU::ADD_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        R.set_flags("z0hc", OP_ADD, R.a, R.z);
        R.a = R.a + R.z;
        break;
    }
}

void CPU::ADD_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.set_flags("z0hc", OP_ADD, R.a, R.z);
        R.a = R.a + R.z;
        break;
    }
}

void CPU::ADC(byte r8) {
    byte x = R.a + r8 + R.get_flag(FLAG_C);
    R.set_flags("z0hc", OP_ADC, R.a, r8);
    R.a = x;
}

void CPU::ADC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        byte x = R.a + R.z + R.get_flag(FLAG_C);
        R.set_flags("z0hc", OP_ADC, R.a, R.z);
        R.a = x;
        break;
    }
}

void CPU::ADC_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        byte x = R.a + R.z + R.get_flag(FLAG_C);
        R.set_flags("z0hc", OP_ADC, R.a, R.z);
        R.a = x;
        break;
    }
}

void CPU::SUB(byte r8) {
    R.set_flags("z1hc", OP_SUB, R.a, r8);
    R.a = R.a - r8;
}

void CPU::SUB_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        R.set_flags("z1hc", OP_SUB, R.a, R.z);
        R.a = R.a - R.z;
        break;
    }
}

void CPU::SUB_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.set_flags("z1hc", OP_SUB, R.a, R.z);
        R.a = R.a - R.z;
        break;
    }
}

void CPU::SBC(byte r8) {
    byte result = R.a - r8 - R.get_flag(FLAG_C);
    R.set_flags("z1hc", OP_SBC, R.a, r8);
    R.a = result;
}

void CPU::SBC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        byte result = R.a - R.z - R.get_flag(FLAG_C);
        R.set_flags("z1hc", OP_SBC, R.a, R.z);
        R.a = result;
        break;
    }
}

void CPU::SBC_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        byte result = R.a - R.z - R.get_flag(FLAG_C);
        R.set_flags("z1hc", OP_SBC, R.a, R.z);
        R.a = result;
        break;
    }
}

void CPU::CP (byte r8) {
    R.set_flags("z1hc", OP_SUB, R.a, r8);
}

void CPU::CP_HLptr () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        R.set_flags("z1hc", OP_SUB, R.a, R.z);
        break;
    }
}

void CPU::CP_d8 () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.set_flags("z1hc", OP_SUB, R.a, R.z);
        break;
    }
}

void CPU::INC(byte& r8) {
    R.set_flags("z0h-", OP_INC, r8);
    r8++;
}

void CPU::INC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        R.set_flags("z0h-", OP_INC, R.z);
        memory.write(R.hl, R.z + 1);
        break;
    }
}

void CPU::DEC(byte& r8) {
        R.set_flags("z1h-", OP_DEC, r8);
    r8--;
}

void CPU::DEC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        R.set_flags("z1h-", OP_DEC, R.z);
        memory.write(R.hl, R.z - 1);
        break;
    }
}

void CPU::AND(byte r8) {
    R.a = R.a & r8;
    R.set_flags("z010", OP_LGC, R.a);
}

void CPU::AND_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        R.a = R.a & R.z;
        R.set_flags("z010", OP_LGC, R.a);
        break;
    }
}

void CPU::AND_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.a = R.a & R.z;
        R.set_flags("z010", OP_LGC, R.a);
        break;
    }
}

void CPU::OR (byte r8) {
    R.a = R.a | r8;
    R.set_flags("z000", OP_LGC, R.a);
}

void CPU::OR_HLptr () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        R.a = R.a | R.z;
        R.set_flags("z000", OP_LGC, R.a);
        break;
    }
}

void CPU::OR_d8 () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.a = R.a | R.z;
        R.set_flags("z000", OP_LGC, R.a);
        break;
    }
}

void CPU::XOR(byte r8) {
    R.a = R.a ^ r8;
    R.set_flags("z000", OP_LGC, R.a);
}

void CPU::XOR_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        R.a = R.a ^ R.z;
        R.set_flags("z000", OP_LGC, R.a);
        break;
    }
}

void CPU::XOR_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.a = R.a ^ R.z;
        R.set_flags("z000", OP_LGC, R.a);
        break;
    }
}

void CPU::CCF() {
    R.set_flags("-00~");
}

void CPU::SCF() {
    R.set_flags("-001");
}

void CPU::DAA() {
    word result = R.a;
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
    R.a = result;
    // R.af &= 0xF0;
    // R.af |= result << 8;
}

void CPU::CPL() {
    R.a = ~R.a;
    R.set_flags("-11-");
}

// --SECTION--16-BIT ARITHMETIC
void CPU::INC(word& r16) {
    switch (tcycles >> 2) {
        case 0:
        r16++;
        break;
    }
}

void CPU::DEC(word& r16) {
    switch (tcycles >> 2) {
        case 0:
        r16--;
        break;
    }
}

void CPU::ADD_HL(word r16) {
    switch (tcycles >> 2) {
        case 0:
        R.set_flags("-0hc", OP_ADD, R.l, r16 & 0xFF);
        R.l = R.l + (r16 & 0xFF);
        break;
        case 1:
        byte x = R.h + (r16 >> 8) + R.get_flag(FLAG_C);
        R.set_flags("-0hc", OP_ADC, R.h, r16 >> 8);
        R.h = x;
        break;
    }
}

void CPU::ADD_SP() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.set_flags("00hc", OP_ADD, R.z, R.spl);
        R.spl = R.z + R.spl;
        break;
        case 2:
        R.w = R.sph + R.get_flag(FLAG_C) + (R.z >> 7) * 0xFF;
        break;
        case 3:
        R.sph = R.w;
        break;
    }
}

// --SECTION-- BIT OPS
void CPU::CBPREFIX() {
    prefixed_fetch = 1;
}
void CPU::RLCA() {
    R.update_flag(FLAG_C, R.a & 0x80);
    R.a = (R.a << 1) | (R.a >> 7);
    R.set_flags("000-");
}

void CPU::RRCA() {
    R.update_flag(FLAG_C, R.a & 1);
    R.a = (R.a << 7) | (R.a >> 1);
    R.set_flags("000-");
}

void CPU::RLA () {
    byte x = (R.a << 1) | (R.get_flag(FLAG_C));
    R.update_flag(FLAG_C, R.a & 0x80);
    R.a = x;
    R.set_flags("000-");
}

void CPU::RRA() {
    byte x = (R.get_flag(FLAG_C) << 7) | (R.a >> 1);
    R.update_flag(FLAG_C, R.a & 1);
    R.a = x;
    R.set_flags("000-");
}

void CPU::RLC (byte& r8) {
    R.update_flag(FLAG_C, r8 & 0x80);
    r8 = (r8 << 1) | (r8 >> 7);
    R.set_flags("z00-", OP_LGC, r8);
}

void CPU::RLC_HLptr () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        RLC(R.z);
        memory.write(R.hl, R.z);
        break;
    }
}

void CPU::RRC (byte& r8) {
    R.update_flag(FLAG_C, r8 & 1);
    r8 = (r8 << 7) | (r8 >> 1);
    R.set_flags("z00-", OP_LGC, r8);
}

void CPU::RRC_HLptr () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        RRC(R.z);
        memory.write(R.hl, R.z);
        break;
    }
}

void CPU::RL  (byte& r8) {
    byte x = (r8 << 1) | (R.get_flag(FLAG_C));
    R.update_flag(FLAG_C, r8 & 0x80);
    r8 = x;
    R.set_flags("z00-", OP_LGC, r8);
}

void CPU::RL_HLptr  () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        RL(R.z);
        memory.write(R.hl, R.z);
        break;
    }
}

void CPU::RR  (byte& r8) {
    byte x = (R.get_flag(FLAG_C) << 7) | (r8 >> 1);
    R.update_flag(FLAG_C, r8 & 1);
    r8 = x;
    R.set_flags("z00-", OP_LGC, r8);
}

void CPU::RR_HLptr  () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        RR(R.z);
        memory.write(R.hl, R.z);
        break;
    }
}

void CPU::SLA (byte& r8) {
    R.update_flag(FLAG_C, r8 & 0x80);
    r8 = r8 << 1;
    R.set_flags("z00-", OP_LGC, r8);
}

void CPU::SLA_HLptr () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        SLA(R.z);
        memory.write(R.hl, R.z);
        break;
    }
}

void CPU::SRA (byte& r8) {
    R.update_flag(FLAG_C, r8 & 1);
    r8 = (r8 >> 1) | (r8 & 0x80);
    R.set_flags("z00-", OP_LGC, r8);
}

void CPU::SRA_HLptr () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        SRA(R.z);
        memory.write(R.hl, R.z);
        break;
    }
}

void CPU::SWAP(byte& r8) {
    r8 = (r8 << 4) | (r8 >> 4);
    R.set_flags("z000", OP_LGC, r8);
}

void CPU::SWAP_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        SWAP(R.z);
        memory.write(R.hl, R.z);
        break;
    }
}

void CPU::SRL (byte& r8) {
    R.update_flag(FLAG_C, r8 & 1);
    r8 = r8 >> 1;
    R.set_flags("z00-", OP_LGC, r8);
}

void CPU::SRL_HLptr () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        SRL(R.z);
        memory.write(R.hl, R.z);
        break;
    }
}

void CPU::BIT (byte u3, byte r8) {
    R.set_flags("z01-", OP_LGC, r8 & (1 << u3));
}   

void CPU::BIT_HLptr (byte u3) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        BIT(u3, R.z);
        break;
    }
}

void CPU::RES (byte u3, byte& r8) {
    r8 = r8 & ~(1 << u3);
}

void CPU::RES_HLptr (byte u3) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        RES(u3, R.z);
        memory.write(R.hl, R.z);
        break;
    }
}

void CPU::SET (byte u3, byte& r8) {
    r8 = r8 | (1 << u3);
}

void CPU::SET_HLptr (byte u3) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.hl);
        break;
        case 1:
        SET(u3, R.z);
        memory.write(R.hl, R.z);
        break;
    }
}


// --SECTION-- JUMPS
void CPU::JP() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.w = memory.read(R.pc++);
        break;
        case 2:
        R.pc = R.wz;
        break;
    }
}

void CPU::JP_HL() {
    R.pc = R.hl;
}

void CPU::JP(byte cc) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.w = memory.read(R.pc++);
        cc_check = R.get_cc(cc);
        current_tcycles -= cc_check ? 0 : 4;
        break;
        case 2:
        if (cc_check)
            R.pc = R.wz;
        break;
    }
}

void CPU::JR() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.wz = R.pc + (sbyte) R.z;
        break;
        case 2:
        R.pc = R.wz;
        break;
    }
}

void CPU::JR(byte cc) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        cc_check = R.get_cc(cc);
        current_tcycles -= cc_check ? 0 : 4;
        break;
        case 1:
        if (cc_check)
            R.wz = R.pc + (sbyte) R.z;
        break;
        case 2:
        R.pc = R.wz;
        break;
    }
}

void CPU::CALL() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.w = memory.read(R.pc++);
        break;
        case 2:
        R.sp--;
        break;
        case 3:
        memory.write(R.sp--, R.pch);
        break;
        case 4:
        memory.write(R.sp, R.pcl);
        R.pc = R.wz;
        break;
    }
}

void CPU::CALL(byte cc) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.pc++);
        break;
        case 1:
        R.w = memory.read(R.pc++);
        cc_check = R.get_cc(cc);
        current_tcycles -= cc_check ? 0 : 12;
        break;
        case 2:
        if (cc_check)
            R.sp--;
        break;
        case 3:
        memory.write(R.sp--, R.pch);
        break;
        case 4:
        memory.write(R.sp, R.pcl);
        R.pc = R.wz;
        break;
    }
}

void CPU::RET() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.sp++);
        break;
        case 1:
        R.w = memory.read(R.sp++);
        break;
        case 2:
        R.pc = R.wz;
        break;
    }
}

void CPU::RET(byte cc) {
    switch (tcycles >> 2) {
        case 0:
        cc_check = R.get_cc(cc);
        current_tcycles -= cc_check ? 0 : 12;
        break;
        case 1:
        if (cc_check)
            R.z = memory.read(R.sp++);
        break;
        case 2:
        R.w = memory.read(R.sp++);
        break;
        case 3:
        R.pc = R.wz;
        break;
    }
}

void CPU::RETI() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory.read(R.sp++);
        break;
        case 1:
        R.w = memory.read(R.sp++);
        break;
        case 2:
        R.pc = R.wz;
        ime = true;
        break;
    }
}

void CPU::RST(byte vector) {
    switch (tcycles >> 2) {
        case 0:
        R.sp--;
        break;
        case 1:
        memory.write(R.sp--, R.pch);
        break;
        case 2:
        memory.write(R.sp, R.pcl);
        R.pc = vector;
        break;
    }
}

void CPU::INT(byte vector) {
    switch (tcycles >> 2) {
        case 0:
        R.pc--;
        break;
        case 1:
        R.sp--;
        break;
        case 2:
        memory.write(R.sp--, R.pch);
        break;
        case 3:
        memory.write(R.sp, R.pcl);
        R.pc = vector;
        break;
    }
}

// --SECTION-- MISC
void CPU::HALT() {
    halted = true;
}

void CPU::STOP() {
    // TODO: still here

}

void CPU::DI() {
    std::cout << "IRQ SERVICING OFF" << std::endl;
    ime = false;
    ime_buffer = 0;
}

void CPU::EI() {
    std::cout << "IRQ SERVICING QUEUED" << std::endl;
    ime_buffer = EI_0;
}

void CPU::NOP() {

}

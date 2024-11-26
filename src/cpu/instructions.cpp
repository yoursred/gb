#include "cpu/cpu.h"


// --SECTION-- 8-BIT LOADS
void CPU::LD(byte r8, byte& r8p) {
    fetch();
    r8p = r8;
}
void CPU::LD(byte& r8) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        fetch();
        r8 = R.z;
        break;
    }
}

void CPU::LD(byte& r8, word r16ptr) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[r16ptr];
        break;
        case 1:
        fetch();
        r8 = R.z;
        break;
    }
}
void CPU::LD(word r16ptr, byte r8) {
    switch (tcycles >> 2) {
        case 0:
        memory[r16ptr] = r8;
        break;
        case 1:
        fetch();
        break;
    }
}

void CPU::LD_A_d16ptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.w = memory[R.pc++];
        break;
        case 2:
        R.z = memory[R.wz];
        break;
        case 3:
        fetch();
        R.a = R.z;
        break;
    }
}
void CPU::LD_d16ptr_A() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.w = memory[R.pc++];
        break;
        case 2:
        R.a = memory[R.wz];
        break;
        case 3:
        fetch();
        break;
    }
}

void CPU::LDH_A_Cptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[0xFF00 | R.c];
        break;
        case 1:
        fetch();
        R.a = R.z;
        break;
    }
}
void CPU::LDH_Cptr_A() {
    switch (tcycles >> 2) {
        case 0:
        memory[0xFF00 | R.c] = R.a;
        break;
        case 1:
        fetch();
        break;
    }
}

void CPU::LDH_A_d8ptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.z = memory[0xFF00 | R.z];
        case 2:
        fetch();
        R.a = R.z;
        break;
    }
}

void CPU::LDH_d8ptr_A() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        memory[0xFF00 | R.z] = R.a;
        case 2:
        fetch();
        break;
    }
}

void CPU::LD_A_HLdptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl--];
        break;
        case 1:
        fetch();
        R.a = R.z;
        break;
    }
}

void CPU::LD_HLdptr_A() {
    switch (tcycles >> 2) {
        case 0:
        memory[R.hl--] = R.a;
        break;
        case 1:
        fetch();
        break;
    }
}

void CPU::LD_A_HLiptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl++];
        break;
        case 1:
        fetch();
        R.a = R.z;
        break;
    }
}

void CPU::LD_HLiptr_A() {
    switch (tcycles >> 2) {
        case 0:
        memory[R.hl++] = R.a;
        break;
        case 1:
        fetch();
        break;
    }
}

// --SECTION--16-BIT LOADS
void CPU::LD(word& r16) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.w = memory[R.pc++];
        break;
        case 2:
        r16 = R.wz;
        fetch();
        break;
    }
}

void CPU::LD_d16ptr_SP() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.w = memory[R.pc++];
        break;
        case 2:
        memory[R.wz++] = R.spl;
        break;
        case 3:
        memory[R.wz] = R.sph;
        break;
        case 4:
        fetch();
        break;
    }
}

void CPU::LD_SP_HL() {
    switch (tcycles >> 2) {
        case 0:
        R.sp = R.hl;
        break;
        case 1:
        fetch();
        break;
    }
}

void CPU::PUSH(word r16) {
    switch (tcycles >> 2) {
        case 0:
        R.sp--;
        case 1:
        memory[R.sp--] = (r16 >> 8);
        break;
        case 2:
        memory[R.sp] = r16 & 0xFF;
        break;
        case 3:
        fetch();
        break;
    }
}

void CPU::POP(word& r16) {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.sp++];
        break;
        case 2:
        R.w = memory[R.sp++];
        break;
        case 3:
        fetch();
        r16 = R.wz;
        break;
    }
}

void CPU::LD_HL_SP() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
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
    fetch();
}

void CPU::ADD_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.set_flags("z0hc", OP_ADD, R.a, R.z);
        R.a = R.a + R.z;
        fetch();
        break;
    }
}

void CPU::ADD_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.set_flags("z0hc", OP_ADD, R.a, R.z);
        R.a = R.a + R.z;
        fetch();
        break;
    }
}

void CPU::ADC(byte r8) {
    R.set_flags("z0hc", OP_ADC, R.a, r8);
    R.a = r8;
    fetch();
}

void CPU::ADC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.set_flags("z0hc", OP_ADC, R.a, R.z);
        R.a += R.z;
        fetch();
        break;
    }
}

void CPU::ADC_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.set_flags("z0hc", OP_ADC, R.a, R.z);
        R.a += R.z;
        fetch();
        break;
    }
}

void CPU::SUB(byte r8) {
    R.set_flags("z1hc", OP_SUB, R.a, r8);
    R.a = R.a - r8;
    fetch();
}

void CPU::SUB_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.set_flags("z1hc", OP_SUB, R.a, R.z);
        R.a = R.a - R.z;
        fetch();
        break;
    }
}

void CPU::SUB_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.set_flags("z1hc", OP_SUB, R.a, R.z);
        R.a = R.a - R.z;
        fetch();
        break;
    }
}

void CPU::SBC(byte r8) {
    byte result = R.a - r8 - R.get_flag(FLAG_C);
    R.set_flags("z1hc", OP_SBC, R.a, r8);
    R.a = result;
    fetch();
}

void CPU::SBC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        byte result = R.a - R.z - R.get_flag(FLAG_C);
        R.set_flags("z1hc", OP_SBC, R.a, R.z);
        R.a = result;
        fetch();
        break;
    }
}

void CPU::SBC_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        byte result = R.a - R.z - R.get_flag(FLAG_C);
        R.set_flags("z1hc", OP_SBC, R.a, R.z);
        R.a = result;
        fetch();
        break;
    }
}

void CPU::CP (byte r8) {
    R.set_flags("z1hc", OP_SUB, R.a, r8);
    fetch();
}

void CPU::CP_HLptr () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.set_flags("z1hc", OP_SUB, R.a, R.z);
        fetch();
        break;
    }
}

void CPU::CP_d8 () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.set_flags("z1hc", OP_SUB, R.a, R.z);
        fetch();
        break;
    }
}

void CPU::INC(byte r8) {
    R.set_flags("z0h-", OP_INC, R.a);
    R.a++;
    fetch();
}

void CPU::INC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.set_flags("z0h-", OP_INC, R.z);
        memory[R.hl] = R.z + 1;
        break;
        case 2:
        fetch();
        break;
    }
}

void CPU::DEC(byte r8) {
    R.set_flags("z1h-", OP_DEC, R.a);
    R.a--;
    fetch();
}

void CPU::DEC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.set_flags("z1h-", OP_DEC, R.z);
        memory[R.hl] = R.z - 1;
        break;
        case 2:
        fetch();
        break;
    }
}

void CPU::AND(byte r8) {
    R.a = R.a & r8;
    R.set_flags("z010", OP_LGC, R.a);
    fetch();
}

void CPU::AND_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.a = R.a & R.z;
        R.set_flags("z010", OP_LGC, R.a);
        fetch();
        break;
    }
}

void CPU::AND_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.a = R.a & R.z;
        R.set_flags("z010", OP_LGC, R.a);
        fetch();
        break;
    }
}

void CPU::OR (byte r8) {
    R.a = R.a | r8;
    R.set_flags("z010", OP_LGC, R.a);
    fetch();
}

void CPU::OR_HLptr () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.a = R.a | R.z;
        R.set_flags("z010", OP_LGC, R.a);
        fetch();
        break;
    }
}

void CPU::OR_d8 () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.a = R.a | R.z;
        R.set_flags("z010", OP_LGC, R.a);
        fetch();
        break;
    }
}

void CPU::XOR(byte r8) {
    R.a = R.a ^ r8;
    R.set_flags("z010", OP_LGC, R.a);
    fetch();
}

void CPU::XOR_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.a = R.a ^ R.z;
        R.set_flags("z010", OP_LGC, R.a);
        fetch();
        break;
    }
}

void CPU::XOR_d8() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.a = R.a ^ R.z;
        R.set_flags("z010", OP_LGC, R.a);
        fetch();
        break;
    }
}

void CPU::CCF() {
    R.set_flags("~00-");
    fetch();
}

void CPU::SCF() {
    R.set_flags("-001");
    fetch();
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
    fetch();
    // R.af &= 0xF0;
    // R.af |= result << 8;
}

void CPU::CPL() {
    R.a = ~R.a;
    R.set_flags("-11-");
    fetch();
}

// --SECTION--16-BIT ARITHMETIC
void CPU::INC(word& r16) {
    switch (tcycles >> 2) {
        case 0:
        r16++;
        break;
        case 1:
        fetch();
        break;
    }
}

void CPU::DEC(word& r16) {
    switch (tcycles >> 2) {
        case 0:
        r16--;
        break;
        case 1:
        fetch();
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
        R.set_flags("-0hc", OP_ADC, R.h, r16 >> 8);
        R.h = R.h + (r16 >> 8);
        fetch();
        break;
    }
}

void CPU::ADD_SP() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.pc++];
        break;
        case 1:
        R.set_flags("00hc", OP_ADD, R.z, R.spl);
        R.z = R.z + R.spl;
        break;
        case 2:
        R.w = R.sph + R.get_flag(FLAG_C) + (R.z >> 7) * 0xFF;
        break;
        case 3:
        R.sp = R.wz;
        break;
    }
}

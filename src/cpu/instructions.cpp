#include "cpu/cpu.h"


// --SECTION-- 8-BIT LOADS
void CPU::LD(byte& r8, byte& r8p) {
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

void CPU::LD(byte& r8, word& r16ptr) {
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
void CPU::LD(word& r16ptr, byte& r8) {
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

void CPU::PUSH(word& r16) {
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
        R.l = R.spl + R.z;
        R.unset_flag(FLAG_Z);
        R.unset_flag(FLAG_N);
        R.update_flag(FLAG_H, (((R.spl & 0xF) + (R.z & 0xF)) & 0x10));
        R.update_flag(FLAG_C, ((R.spl + R.z) > 0xFF));
        break;
        case 2:
        R.h = R.sph + R.get_flag(FLAG_C) + !!(R.z & 0x80) * 0xFF;
        fetch();
        break;
    }
}

 // --SECTION-- 8-BIT ARITHMETIC
void CPU::ADD(byte& r8) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, (((R.a & 0xf) + (r8 & 0xf)) & 0x10));
    R.update_flag(FLAG_C, (R.a + r8) > 0xFF);
    R.a += r8;
    R.update_flag(FLAG_Z, !R.a);
    fetch();
}

void CPU::ADD_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.unset_flag(FLAG_N);
        R.update_flag(FLAG_H, (((R.a & 0xf) + (R.z & 0xf)) & 0x10));
        R.update_flag(FLAG_C, (R.a + R.z) > 0xFF);
        R.a += R.z;
        R.update_flag(FLAG_Z, !(R.a));
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
        R.update_flag(FLAG_Z, !(R.a + R.z));
        R.unset_flag(FLAG_N);
        R.update_flag(FLAG_H, (((R.a & 0xf) + (R.z & 0xf)) & 0x10));
        R.update_flag(FLAG_C, (R.a + R.z) > 0xFF);
        R.a += R.z;
        fetch();
        break;
    }
}

void CPU::ADC(byte& r8) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, (((R.a & 0xf) + (r8 & 0xf) + R.get_flag(FLAG_C)) & 0x10));
    R.update_flag(FLAG_C, (R.a + r8 + R.get_flag(FLAG_C)) > 0xFF);
    R.a += r8;
    R.update_flag(FLAG_Z, !R.a);
    fetch();
}

void CPU::ADC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.unset_flag(FLAG_N);
        R.update_flag(FLAG_H, (((R.a & 0xf) + (R.z & 0xf) + R.get_flag(FLAG_C)) & 0x10));
        R.update_flag(FLAG_C, (R.a + R.z + R.get_flag(FLAG_C)) > 0xFF);
        R.a += R.z;
        R.update_flag(FLAG_Z, !(R.a));
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
        R.unset_flag(FLAG_N);
        R.update_flag(FLAG_H, (((R.a & 0xf) + (R.z & 0xf) + R.get_flag(FLAG_C)) & 0x10));
        R.update_flag(FLAG_C, (R.a + R.z + R.get_flag(FLAG_C)) > 0xFF);
        R.a += R.z;
        R.update_flag(FLAG_Z, !(R.a));
        fetch();
        break;
    }
}

void CPU::SUB(byte& r8) {
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, (((R.a & 0xf) - (r8 & 0xf)) & 0x10));
    R.update_flag(FLAG_C, R.a < r8);
    R.a -= r8;
    R.update_flag(FLAG_Z, !R.a);
    fetch();
}

void CPU::SUB_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.set_flag(FLAG_N);
        R.update_flag(FLAG_H, (((R.a & 0xf) - (R.z & 0xf)) & 0x10));
        R.update_flag(FLAG_C, R.a < R.z);
        R.a -= R.z;
        R.update_flag(FLAG_Z, !(R.a));
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
        R.update_flag(FLAG_Z, !(R.a + R.z));
        R.unset_flag(FLAG_N);
        R.update_flag(FLAG_H, ((R.a & 0xf) + (R.z & 0xf)) & 0x10);
        R.update_flag(FLAG_C, (R.a + R.z) > 0xFF);
        R.a -= R.z;
        fetch();
        break;
    }
}

void CPU::SBC(byte& r8) {
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, (R.a & 0xf) < ((r8 & 0xf) + R.get_flag(FLAG_C)));
    R.update_flag(FLAG_C, R.a < (r8 + R.get_flag(FLAG_C)));
    R.a = R.a - r8 - R.get_flag(FLAG_C);
    R.update_flag(FLAG_Z, !R.a);
    fetch();
}

void CPU::SBC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.set_flag(FLAG_N);
        R.update_flag(FLAG_H, (R.a & 0xf) < ((R.z & 0xf) + R.get_flag(FLAG_C)));
        R.update_flag(FLAG_C, R.a < (R.z + R.get_flag(FLAG_C)));
        R.a = R.a - R.z - R.get_flag(FLAG_C);
        R.update_flag(FLAG_Z, !(R.a));
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
        R.set_flag(FLAG_N);
        R.update_flag(FLAG_H, (R.a & 0xf) < ((R.z & 0xf) + R.get_flag(FLAG_C)));
        R.update_flag(FLAG_C, R.a < (R.z + R.get_flag(FLAG_C)));
        R.a = R.a - R.z - R.get_flag(FLAG_C);
        R.update_flag(FLAG_Z, !(R.a));
        fetch();
        break;
    }
}

void CPU::CP (byte& r8) {
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, (((R.a & 0xf) - (r8 & 0xf)) & 0x10));
    R.update_flag(FLAG_C, R.a < r8);
    R.update_flag(FLAG_Z, !(R.a - r8));
    fetch();
}

void CPU::CP_HLptr () {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        R.set_flag(FLAG_N);
        R.update_flag(FLAG_H, (((R.a & 0xf) - (R.z & 0xf)) & 0x10));
        R.update_flag(FLAG_C, R.a < R.z);
        R.update_flag(FLAG_Z, !(R.a - R.z));
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
        R.set_flag(FLAG_N);
        R.update_flag(FLAG_H, (((R.a & 0xf) - (R.z & 0xf)) & 0x10));
        R.update_flag(FLAG_C, R.a < R.z);
        R.update_flag(FLAG_Z, !(R.a - R.z));
        fetch();
        break;
    }
}

void CPU::INC(byte& r8) {
    R.unset_flag(FLAG_N);
    R.update_flag(FLAG_H, ((r8 & 0xf) + 1) & 0x10);
    R.update_flag(FLAG_Z, !(++R.a));
    fetch();
}

void CPU::INC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        memory[R.hl] = R.z + 1;
        R.unset_flag(FLAG_N);
        R.update_flag(FLAG_H, ((R.z & 0xF) + 1) & 0x10);
        R.update_flag(FLAG_Z, !(++R.z));
        break;
        case 2:
        fetch();
        break;
    }
}

void CPU::DEC(byte& r8) {
    R.set_flag(FLAG_N);
    R.update_flag(FLAG_H, ((r8 & 0xf) - 1) & 0x10);
    R.update_flag(FLAG_Z, !(--R.a));
    fetch();
}

void CPU::DEC_HLptr() {
    switch (tcycles >> 2) {
        case 0:
        R.z = memory[R.hl];
        break;
        case 1:
        memory[R.hl] = R.z - 1;
        R.unset_flag(FLAG_N);
        R.update_flag(FLAG_H, ((R.z & 0xF) - 1) & 0x10);
        R.update_flag(FLAG_Z, !(--R.z));
        break;
        case 2:
        fetch();
        break;
    }
}

void CPU::AND(byte& r8) {

}

void CPU::AND_HLptr() {

}

void CPU::AND_d8() {

}

void CPU::OR (byte& r8) {

}

void CPU::OR_HLptr () {

}

void CPU::OR_d8 () {

}

void CPU::XOR(byte& r8) {

}

void CPU::XOR_HLptr() {

}

void CPU::XOR_d8() {

}

void CPU::CCF() {

}

void CPU::SCF() {

}

void CPU::CPL() {

}

void CPU::DAA() {

}


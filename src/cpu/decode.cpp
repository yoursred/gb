#include "cpu/cpu.h"
#include <iostream>

// TODO: Come up with a better decoder

void CPU::decode() {
    switch (CPU::opcode) {
        // case (0x00): CPU::NOP();break; // There shouldn't be a need to call this
        case (0x01): CPU::LD(R.bc, working_word);break;
        case (0x02): 
            CPU::LD(memory[R.bc], R_A);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x03): CPU::INC(R.bc);break;
        case (0x04): CPU::INC(R_B);break;
        case (0x05): CPU::DEC(R_B);break;
        case (0x06): CPU::LD(R_B, &working_byte);break;
        case (0x07): CPU::RLC(R_A); R.unset_flag(FLAG_Z);break; // TODO: And this one
        case (0x08): CPU::LD16SP(working_word);break;
        case (0x09): CPU::ADD(R.bc);break;
        case (0x0a): CPU::LD(R_A, memory[R.bc]);break;
        case (0x0b): CPU::DEC(R.bc);break;
        case (0x0c): CPU::INC(R_C);break;
        case (0x0d): CPU::DEC(R_C);break;
        case (0x0e): CPU::LD(R_C, &working_byte);break;
        case (0x0f): CPU::RRC(R_A); R.unset_flag(FLAG_Z);break; // TODO: This one too
        case (0x10): CPU::STOP();break;
        case (0x11): CPU::LD(R.de, working_word);break;
        case (0x12): 
            CPU::LD(memory[R.de], R_A);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x13): CPU::INC(R.de);break;
        case (0x14): CPU::INC(R_D);break;
        case (0x15): CPU::DEC(R_D);break;
        case (0x16): CPU::LD(R_D, &working_byte);break;
        case (0x17): CPU::RL(R_A); R.unset_flag(FLAG_Z);break; // TODO: I give up
        case (0x18): CPU::JR(working_byte);break;
        case (0x19): CPU::ADD(R.de);break;
        case (0x1a): CPU::LD(R_A, memory[R.de]);break;
        case (0x1b): CPU::DEC(R.de);break;
        case (0x1c): CPU::INC(R_E);break;
        case (0x1d): CPU::DEC(R_E);break;
        case (0x1e): CPU::LD(R_E, &working_byte);break;
        case (0x1f): CPU::RR(R_A); R.unset_flag(FLAG_Z);break; // TODO: re-add RRA because it doesn't set zero for some reason
        case (0x20): CPU::JRC(CC_NZ, working_byte);break;
        case (0x21): CPU::LD(R.hl, working_word);break;
        case (0x22): 
            CPU::LDI(memory[R.hl], R_A);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x23): CPU::INC(R.hl);break;
        case (0x24): CPU::INC(R_H);break;
        case (0x25): CPU::DEC(R_H);break;
        case (0x26): CPU::LD(R_H, &working_byte);break;
        case (0x27): CPU::DAA();break;
        case (0x28): CPU::JRC(CC_Z, working_byte);break;
        case (0x29): CPU::ADD(R.hl);break;
        case (0x2a): CPU::LDI(R_A, memory[R.hl]);break;
        case (0x2b): CPU::DEC(R.hl);break;
        case (0x2c): CPU::INC(R_L);break;
        case (0x2d): CPU::DEC(R_L);break;
        case (0x2e): CPU::LD(R_L, &working_byte);break;
        case (0x2f): CPU::CPL();break;
        case (0x30): CPU::JRC(CC_NC, working_byte);break;
        case (0x31): CPU::LD(R.sp, working_word);break;
        case (0x32): 
            CPU::LDD(memory[R.hl], R_A);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x33): CPU::INC(R.sp);break;
        case (0x34): 
            CPU::INC(memory[R.hl]);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x35): 
            CPU::DEC(memory[R.hl]);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x36): 
            CPU::LD(memory[R.hl], &working_byte);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x37): CPU::SCF();break;
        case (0x38): CPU::JRC(CC_C, working_byte);break;
        case (0x39): CPU::ADD(R.sp);break;
        case (0x3a): CPU::LDD(R_A, memory[R.hl]);break;
        case (0x3b): CPU::DEC(R.sp);break;
        case (0x3c): CPU::INC(R_A);break;
        case (0x3d): CPU::DEC(R_A);break;
        case (0x3e): CPU::LD(R_A, &working_byte);break;
        case (0x3f): CPU::CCF();break;
        case (0x40): CPU::LD(R_B, R_B);break;
        case (0x41): CPU::LD(R_B, R_C);break;
        case (0x42): CPU::LD(R_B, R_D);break;
        case (0x43): CPU::LD(R_B, R_E);break;
        case (0x44): CPU::LD(R_B, R_H);break;
        case (0x45): CPU::LD(R_B, R_L);break;
        case (0x46): CPU::LD(R_B, memory[R.hl]);break;
        case (0x47): CPU::LD(R_B, R_A);break;
        case (0x48): CPU::LD(R_C, R_B);break;
        case (0x49): CPU::LD(R_C, R_C);break;
        case (0x4a): CPU::LD(R_C, R_D);break;
        case (0x4b): CPU::LD(R_C, R_E);break;
        case (0x4c): CPU::LD(R_C, R_H);break;
        case (0x4d): CPU::LD(R_C, R_L);break;
        case (0x4e): CPU::LD(R_C, memory[R.hl]);break;
        case (0x4f): CPU::LD(R_C, R_A);break;         
        case (0x50): CPU::LD(R_D, R_B);break;
        case (0x51): CPU::LD(R_D, R_C);break;
        case (0x52): CPU::LD(R_D, R_D);break;
        case (0x53): CPU::LD(R_D, R_E);break;
        case (0x54): CPU::LD(R_D, R_H);break;
        case (0x55): CPU::LD(R_D, R_L);break;
        case (0x56): CPU::LD(R_D, memory[R.hl]);break;
        case (0x57): CPU::LD(R_D, R_A);break;         
        case (0x58): CPU::LD(R_E, R_B);break;
        case (0x59): CPU::LD(R_E, R_C);break;
        case (0x5a): CPU::LD(R_E, R_D);break;
        case (0x5b): CPU::LD(R_E, R_E);break;
        case (0x5c): CPU::LD(R_E, R_H);break;
        case (0x5d): CPU::LD(R_E, R_L);break;
        case (0x5e): CPU::LD(R_E, memory[R.hl]);break;
        case (0x5f): CPU::LD(R_E, R_A);break;         
        case (0x60): CPU::LD(R_H, R_B);break;
        case (0x61): CPU::LD(R_H, R_C);break;
        case (0x62): CPU::LD(R_H, R_D);break;
        case (0x63): CPU::LD(R_H, R_E);break;
        case (0x64): CPU::LD(R_H, R_H);break;
        case (0x65): CPU::LD(R_H, R_L);break;
        case (0x66): CPU::LD(R_H, memory[R.hl]);break;
        case (0x67): CPU::LD(R_H, R_A);break;         
        case (0x68): CPU::LD(R_L, R_B);break;
        case (0x69): CPU::LD(R_L, R_C);break;
        case (0x6a): CPU::LD(R_L, R_D);break;
        case (0x6b): CPU::LD(R_L, R_E);break;
        case (0x6c): CPU::LD(R_L, R_H);break;
        case (0x6d): CPU::LD(R_L, R_L);break;
        case (0x6e): CPU::LD(R_L, memory[R.hl]);break;
        case (0x6f): CPU::LD(R_L, R_A);break;         
        case (0x70): 
            CPU::LD(memory[R.hl], R_B);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x71): 
            CPU::LD(memory[R.hl], R_C);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x72): 
            CPU::LD(memory[R.hl], R_D);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x73): 
            CPU::LD(memory[R.hl], R_E);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x74): 
            CPU::LD(memory[R.hl], R_H);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x75): 
            CPU::LD(memory[R.hl], R_L);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x76): CPU::HALT();break;
        case (0x77): 
            CPU::LD(memory[R.hl], R_A);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;         
        case (0x78): CPU::LD(R_A, R_B);break;
        case (0x79): CPU::LD(R_A, R_C);break;
        case (0x7a): CPU::LD(R_A, R_D);break;
        case (0x7b): CPU::LD(R_A, R_E);break;
        case (0x7c): CPU::LD(R_A, R_H);break;
        case (0x7d): CPU::LD(R_A, R_L);break;
        case (0x7e): CPU::LD(R_A, memory[R.hl]);break;
        case (0x7f): CPU::LD(R_A, R_A);break;         
        case (0x80): CPU::ADD(R_B);break;
        case (0x81): CPU::ADD(R_C);break;
        case (0x82): CPU::ADD(R_D);break;
        case (0x83): CPU::ADD(R_E);break;
        case (0x84): CPU::ADD(R_H);break;
        case (0x85): CPU::ADD(R_L);break;
        case (0x86): CPU::ADD(memory[R.hl]);break;
        case (0x87): CPU::ADD(R_A);break;        
        case (0x88): CPU::ADC(R_B);break;
        case (0x89): CPU::ADC(R_C);break;
        case (0x8a): CPU::ADC(R_D);break;
        case (0x8b): CPU::ADC(R_E);break;
        case (0x8c): CPU::ADC(R_H);break;
        case (0x8d): CPU::ADC(R_L);break;
        case (0x8e): CPU::ADC(memory[R.hl]);break;
        case (0x8f): CPU::ADC(R_A);break;
        case (0x90): CPU::SUB(R_B);break;
        case (0x91): CPU::SUB(R_C);break;
        case (0x92): CPU::SUB(R_D);break;
        case (0x93): CPU::SUB(R_E);break;
        case (0x94): CPU::SUB(R_H);break;
        case (0x95): CPU::SUB(R_L);break;
        case (0x96): CPU::SUB(memory[R.hl]);break;
        case (0x97): CPU::SUB(R_A);break;        
        case (0x98): CPU::SBC(R_B);break;
        case (0x99): CPU::SBC(R_C);break;
        case (0x9a): CPU::SBC(R_D);break;
        case (0x9b): CPU::SBC(R_E);break;
        case (0x9c): CPU::SBC(R_H);break;
        case (0x9d): CPU::SBC(R_L);break;
        case (0x9e): CPU::SBC(memory[R.hl]);break;
        case (0x9f): CPU::SBC(R_A);break;        
        case (0xa0): CPU::AND(R_B);break;
        case (0xa1): CPU::AND(R_C);break;
        case (0xa2): CPU::AND(R_D);break;
        case (0xa3): CPU::AND(R_E);break;
        case (0xa4): CPU::AND(R_H);break;
        case (0xa5): CPU::AND(R_L);break;
        case (0xa6): CPU::AND(memory[R.hl]);break;
        case (0xa7): CPU::AND(R_A);break;        
        case (0xa8): CPU::XOR(R_B);break;
        case (0xa9): CPU::XOR(R_C);break;
        case (0xaa): CPU::XOR(R_D);break;
        case (0xab): CPU::XOR(R_E);break;
        case (0xac): CPU::XOR(R_H);break;
        case (0xad): CPU::XOR(R_L);break;
        case (0xae): CPU::XOR(memory[R.hl]);break;
        case (0xaf): CPU::XOR(R_A);break;        
        case (0xb0): CPU::OR(R_B);break;
        case (0xb1): CPU::OR(R_C);break;
        case (0xb2): CPU::OR(R_D);break;
        case (0xb3): CPU::OR(R_E);break;
        case (0xb4): CPU::OR(R_H);break;
        case (0xb5): CPU::OR(R_L);break;
        case (0xb6): CPU::OR(memory[R.hl]);break;
        case (0xb7): CPU::OR(R_A);break;        
        case (0xb8): CPU::CP(R_B);break;
        case (0xb9): CPU::CP(R_C);break;
        case (0xba): CPU::CP(R_D);break;
        case (0xbb): CPU::CP(R_E);break;
        case (0xbc): CPU::CP(R_H);break;
        case (0xbd): CPU::CP(R_L);break;
        case (0xbe): CPU::CP(memory[R.hl]);break;
        case (0xbf): CPU::CP(R_A);break;        
        case (0xc0): CPU::RETC(CC_NZ);break;
        case (0xc1): CPU::POP(R.bc);break;
        case (0xc2): CPU::JPC(CC_NZ, working_word);break;
        case (0xc3): CPU::JP(working_word);break;
        case (0xc4): CPU::CALLC(CC_NZ, working_word);break;
        case (0xc5): CPU::PUSH(R.bc);break;
        case (0xc6): CPU::ADD(&working_byte);break;
        case (0xc7): CPU::RST(0x00);break;
        case (0xc8): CPU::RETC(CC_Z);break;
        case (0xc9): CPU::RET();break;
        case (0xca): CPU::JPC(CC_Z, working_word);break;
        case (0xcc): CPU::CALLC(CC_Z, working_word);break;
        case (0xcd): CPU::CALL(working_word);break;
        case (0xce): CPU::ADC(&working_byte);break;
        case (0xcf): CPU::RST(0x08);break;
        case (0xd0): CPU::RETC(CC_NC);break;
        case (0xd1): CPU::POP(R.de);break;
        case (0xd2): CPU::JPC(CC_NC, working_word);break;
        case (0xd4): CPU::CALLC(CC_NC, working_word);break;
        case (0xd5): CPU::PUSH(R.de);break;
        case (0xd6): CPU::SUB(&working_byte);break;
        case (0xd7): CPU::RST(0x10);break;
        case (0xd8): CPU::RETC(CC_C);break;
        case (0xd9): CPU::RETI();break;
        case (0xda): CPU::JPC(CC_C, working_word);break;
        case (0xdc): CPU::CALLC(CC_C, working_word);break;
        case (0xde): CPU::SBC(&working_byte);break;
        case (0xdf): CPU::RST(0x18);break;
        case (0xe0): 
            CPU::LD(memory[0xFF00 + working_byte], R_A);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xe1): CPU::POP(R.hl);break;
        case (0xe2): 
            CPU::LD(memory[0xFF00 + R_C], R_A);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xe5): CPU::PUSH(R.hl);break;
        case (0xe6): CPU::AND(&working_byte);break;
        case (0xe7): CPU::RST(0x20);break;
        case (0xe8): CPU::ADD_SP((sbyte) working_byte);break;
        case (0xe9): CPU::JP(R.hl);break;
        case (0xea): 
            CPU::LD(memory[working_word], R_A);
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xee): CPU::XOR(&working_byte);break;
        case (0xef): CPU::RST(0x28);break;
        case (0xf0): CPU::LD(R_A, memory[0xFF00 + working_byte]);break;
        case (0xf1): CPU::POP(R.af);break;
        case (0xf2): CPU::LD(R_A, memory[0xFF00 + R_C]);break;
        case (0xf3): CPU::DI();break;
        case (0xf5): CPU::PUSH(R.af);break;
        case (0xf6): CPU::OR(&working_byte);break;
        case (0xf7): CPU::RST(0x30);break;
        case (0xf8): CPU::LDHL();break;
        case (0xf9): CPU::LD(R.sp, R.hl);break;
        case (0xfa): CPU::LD(R_A, memory[working_word]); break;
        case (0xfb): CPU::EI();break;
        case (0xfe): CPU::CP(&working_byte);break;
        case (0xff): CPU::RST(0x38);break;
    }
}
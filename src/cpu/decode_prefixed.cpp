#include "cpu/cpu.h"

void CPU::decode_prefixed() {
    switch (CPU::working_byte) {
        case (0x00): CPU::RLC(R_B); break;
        case (0x01): CPU::RLC(R_C); break;
        case (0x02): CPU::RLC(R_D); break;
        case (0x03): CPU::RLC(R_E); break;
        case (0x04): CPU::RLC(R_H); break;
        case (0x05): CPU::RLC(R_L); break;
        case (0x06): 
            CPU::RLC(memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x07): CPU::RLC(R_A); break;
        case (0x08): CPU::RRC(R_B); break;
        case (0x09): CPU::RRC(R_C); break;
        case (0x0a): CPU::RRC(R_D); break;
        case (0x0b): CPU::RRC(R_E); break;
        case (0x0c): CPU::RRC(R_H); break;
        case (0x0d): CPU::RRC(R_L); break;
        case (0x0e): 
            CPU::RRC(memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x0f): CPU::RRC(R_A); break;
        case (0x10): CPU::RL(R_B); break;
        case (0x11): CPU::RL(R_C); break;
        case (0x12): CPU::RL(R_D); break;
        case (0x13): CPU::RL(R_E); break;
        case (0x14): CPU::RL(R_H); break;
        case (0x15): CPU::RL(R_L); break;
        case (0x16): 
            CPU::RL(memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x17): CPU::RL(R_A); break;
        case (0x18): CPU::RR(R_B); break;
        case (0x19): CPU::RR(R_C); break;
        case (0x1a): CPU::RR(R_D); break;
        case (0x1b): CPU::RR(R_E); break;
        case (0x1c): CPU::RR(R_H); break;
        case (0x1d): CPU::RR(R_L); break;
        case (0x1e): 
            CPU::RR(memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x1f): CPU::RR(R_A); break;
        case (0x20): CPU::SLA(R_B); break;
        case (0x21): CPU::SLA(R_C); break;
        case (0x22): CPU::SLA(R_D); break;
        case (0x23): CPU::SLA(R_E); break;
        case (0x24): CPU::SLA(R_H); break;
        case (0x25): CPU::SLA(R_L); break;
        case (0x26): 
            CPU::SLA(memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x27): CPU::SLA(R_A); break;
        case (0x28): CPU::SRA(R_B); break;
        case (0x29): CPU::SRA(R_C); break;
        case (0x2a): CPU::SRA(R_D); break;
        case (0x2b): CPU::SRA(R_E); break;
        case (0x2c): CPU::SRA(R_H); break;
        case (0x2d): CPU::SRA(R_L); break;
        case (0x2e): 
            CPU::SRA(memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x2f): CPU::SRA(R_A); break;
        case (0x30): CPU::SWAP(R_B); break;
        case (0x31): CPU::SWAP(R_C); break;
        case (0x32): CPU::SWAP(R_D); break;
        case (0x33): CPU::SWAP(R_E); break;
        case (0x34): CPU::SWAP(R_H); break;
        case (0x35): CPU::SWAP(R_L); break;
        case (0x36): 
            CPU::SWAP(memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x37): CPU::SWAP(R_A); break;
        case (0x38): CPU::SRL(R_B); break;
        case (0x39): CPU::SRL(R_C); break;
        case (0x3a): CPU::SRL(R_D); break;
        case (0x3b): CPU::SRL(R_E); break;
        case (0x3c): CPU::SRL(R_H); break;
        case (0x3d): CPU::SRL(R_L); break;
        case (0x3e): 
            CPU::SRL(memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x3f): CPU::SRL(R_A); break;
        case (0x40): CPU::BIT(0, R_B); break;
        case (0x41): CPU::BIT(0, R_C); break;
        case (0x42): CPU::BIT(0, R_D); break;
        case (0x43): CPU::BIT(0, R_E); break;
        case (0x44): CPU::BIT(0, R_H); break;
        case (0x45): CPU::BIT(0, R_L); break;
        case (0x46): CPU::BIT(0, memory[R.hl]); break;
        case (0x47): CPU::BIT(0, R_A); break;
        case (0x48): CPU::BIT(1, R_B); break;
        case (0x49): CPU::BIT(1, R_C); break;
        case (0x4a): CPU::BIT(1, R_D); break;
        case (0x4b): CPU::BIT(1, R_E); break;
        case (0x4c): CPU::BIT(1, R_H); break;
        case (0x4d): CPU::BIT(1, R_L); break;
        case (0x4e): CPU::BIT(1, memory[R.hl]); break;
        case (0x4f): CPU::BIT(1, R_A); break;
        case (0x50): CPU::BIT(2, R_B); break;
        case (0x51): CPU::BIT(2, R_C); break;
        case (0x52): CPU::BIT(2, R_D); break;
        case (0x53): CPU::BIT(2, R_E); break;
        case (0x54): CPU::BIT(2, R_H); break;
        case (0x55): CPU::BIT(2, R_L); break;
        case (0x56): CPU::BIT(2, memory[R.hl]); break;
        case (0x57): CPU::BIT(2, R_A); break;
        case (0x58): CPU::BIT(3, R_B); break;
        case (0x59): CPU::BIT(3, R_C); break;
        case (0x5a): CPU::BIT(3, R_D); break;
        case (0x5b): CPU::BIT(3, R_E); break;
        case (0x5c): CPU::BIT(3, R_H); break;
        case (0x5d): CPU::BIT(3, R_L); break;
        case (0x5e): CPU::BIT(3, memory[R.hl]); break;
        case (0x5f): CPU::BIT(3, R_A); break;
        case (0x60): CPU::BIT(4, R_B); break;
        case (0x61): CPU::BIT(4, R_C); break;
        case (0x62): CPU::BIT(4, R_D); break;
        case (0x63): CPU::BIT(4, R_E); break;
        case (0x64): CPU::BIT(4, R_H); break;
        case (0x65): CPU::BIT(4, R_L); break;
        case (0x66): CPU::BIT(4, memory[R.hl]); break;
        case (0x67): CPU::BIT(4, R_A); break;
        case (0x68): CPU::BIT(5, R_B); break;
        case (0x69): CPU::BIT(5, R_C); break;
        case (0x6a): CPU::BIT(5, R_D); break;
        case (0x6b): CPU::BIT(5, R_E); break;
        case (0x6c): CPU::BIT(5, R_H); break;
        case (0x6d): CPU::BIT(5, R_L); break;
        case (0x6e): CPU::BIT(5, memory[R.hl]); break;
        case (0x6f): CPU::BIT(5, R_A); break;
        case (0x70): CPU::BIT(6, R_B); break;
        case (0x71): CPU::BIT(6, R_C); break;
        case (0x72): CPU::BIT(6, R_D); break;
        case (0x73): CPU::BIT(6, R_E); break;
        case (0x74): CPU::BIT(6, R_H); break;
        case (0x75): CPU::BIT(6, R_L); break;
        case (0x76): CPU::BIT(6, memory[R.hl]); break;
        case (0x77): CPU::BIT(6, R_A); break;
        case (0x78): CPU::BIT(7, R_B); break;
        case (0x79): CPU::BIT(7, R_C); break;
        case (0x7a): CPU::BIT(7, R_D); break;
        case (0x7b): CPU::BIT(7, R_E); break;
        case (0x7c): CPU::BIT(7, R_H); break;
        case (0x7d): CPU::BIT(7, R_L); break;
        case (0x7e): CPU::BIT(7, memory[R.hl]); break;
        case (0x7f): CPU::BIT(7, R_A); break;
        case (0x80): CPU::RES(0, R_B); break;
        case (0x81): CPU::RES(0, R_C); break;
        case (0x82): CPU::RES(0, R_D); break;
        case (0x83): CPU::RES(0, R_E); break;
        case (0x84): CPU::RES(0, R_H); break;
        case (0x85): CPU::RES(0, R_L); break;
        case (0x86): 
            CPU::RES(0, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x87): CPU::RES(0, R_A); break;
        case (0x88): CPU::RES(1, R_B); break;
        case (0x89): CPU::RES(1, R_C); break;
        case (0x8a): CPU::RES(1, R_D); break;
        case (0x8b): CPU::RES(1, R_E); break;
        case (0x8c): CPU::RES(1, R_H); break;
        case (0x8d): CPU::RES(1, R_L); break;
        case (0x8e): 
            CPU::RES(1, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x8f): CPU::RES(1, R_A); break;
        case (0x90): CPU::RES(2, R_B); break;
        case (0x91): CPU::RES(2, R_C); break;
        case (0x92): CPU::RES(2, R_D); break;
        case (0x93): CPU::RES(2, R_E); break;
        case (0x94): CPU::RES(2, R_H); break;
        case (0x95): CPU::RES(2, R_L); break;
        case (0x96): 
            CPU::RES(2, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x97): CPU::RES(2, R_A); break;
        case (0x98): CPU::RES(3, R_B); break;
        case (0x99): CPU::RES(3, R_C); break;
        case (0x9a): CPU::RES(3, R_D); break;
        case (0x9b): CPU::RES(3, R_E); break;
        case (0x9c): CPU::RES(3, R_H); break;
        case (0x9d): CPU::RES(3, R_L); break;
        case (0x9e): 
            CPU::RES(3, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0x9f): CPU::RES(3, R_A); break;
        case (0xa0): CPU::RES(4, R_B); break;
        case (0xa1): CPU::RES(4, R_C); break;
        case (0xa2): CPU::RES(4, R_D); break;
        case (0xa3): CPU::RES(4, R_E); break;
        case (0xa4): CPU::RES(4, R_H); break;
        case (0xa5): CPU::RES(4, R_L); break;
        case (0xa6): 
            CPU::RES(4, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xa7): CPU::RES(4, R_A); break;
        case (0xa8): CPU::RES(5, R_B); break;
        case (0xa9): CPU::RES(5, R_C); break;
        case (0xaa): CPU::RES(5, R_D); break;
        case (0xab): CPU::RES(5, R_E); break;
        case (0xac): CPU::RES(5, R_H); break;
        case (0xad): CPU::RES(5, R_L); break;
        case (0xae): 
            CPU::RES(5, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xaf): CPU::RES(5, R_A); break;
        case (0xb0): CPU::RES(6, R_B); break;
        case (0xb1): CPU::RES(6, R_C); break;
        case (0xb2): CPU::RES(6, R_D); break;
        case (0xb3): CPU::RES(6, R_E); break;
        case (0xb4): CPU::RES(6, R_H); break;
        case (0xb5): CPU::RES(6, R_L); break;
        case (0xb6): 
            CPU::RES(6, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xb7): CPU::RES(6, R_A); break;
        case (0xb8): CPU::RES(7, R_B); break;
        case (0xb9): CPU::RES(7, R_C); break;
        case (0xba): CPU::RES(7, R_D); break;
        case (0xbb): CPU::RES(7, R_E); break;
        case (0xbc): CPU::RES(7, R_H); break;
        case (0xbd): CPU::RES(7, R_L); break;
        case (0xbe): 
            CPU::RES(7, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xbf): CPU::RES(7, R_A); break;
        case (0xc0): CPU::SET(0, R_B); break;
        case (0xc1): CPU::SET(0, R_C); break;
        case (0xc2): CPU::SET(0, R_D); break;
        case (0xc3): CPU::SET(0, R_E); break;
        case (0xc4): CPU::SET(0, R_H); break;
        case (0xc5): CPU::SET(0, R_L); break;
        case (0xc6): 
            CPU::SET(0, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xc7): CPU::SET(0, R_A); break;
        case (0xc8): CPU::SET(1, R_B); break;
        case (0xc9): CPU::SET(1, R_C); break;
        case (0xca): CPU::SET(1, R_D); break;
        case (0xcb): CPU::SET(1, R_E); break;
        case (0xcc): CPU::SET(1, R_H); break;
        case (0xcd): CPU::SET(1, R_L); break;
        case (0xce): 
            CPU::SET(1, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xcf): CPU::SET(1, R_A); break;
        case (0xd0): CPU::SET(2, R_B); break;
        case (0xd1): CPU::SET(2, R_C); break;
        case (0xd2): CPU::SET(2, R_D); break;
        case (0xd3): CPU::SET(2, R_E); break;
        case (0xd4): CPU::SET(2, R_H); break;
        case (0xd5): CPU::SET(2, R_L); break;
        case (0xd6): 
            CPU::SET(2, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xd7): CPU::SET(2, R_A); break;
        case (0xd8): CPU::SET(3, R_B); break;
        case (0xd9): CPU::SET(3, R_C); break;
        case (0xda): CPU::SET(3, R_D); break;
        case (0xdb): CPU::SET(3, R_E); break;
        case (0xdc): CPU::SET(3, R_H); break;
        case (0xdd): CPU::SET(3, R_L); break;
        case (0xde): 
            CPU::SET(3, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xdf): CPU::SET(3, R_A); break;
        case (0xe0): CPU::SET(4, R_B); break;
        case (0xe1): CPU::SET(4, R_C); break;
        case (0xe2): CPU::SET(4, R_D); break;
        case (0xe3): CPU::SET(4, R_E); break;
        case (0xe4): CPU::SET(4, R_H); break;
        case (0xe5): CPU::SET(4, R_L); break;
        case (0xe6): 
            CPU::SET(4, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xe7): CPU::SET(4, R_A); break;
        case (0xe8): CPU::SET(5, R_B); break;
        case (0xe9): CPU::SET(5, R_C); break;
        case (0xea): CPU::SET(5, R_D); break;
        case (0xeb): CPU::SET(5, R_E); break;
        case (0xec): CPU::SET(5, R_H); break;
        case (0xed): CPU::SET(5, R_L); break;
        case (0xee): 
            CPU::SET(5, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xef): CPU::SET(5, R_A); break;
        case (0xf0): CPU::SET(6, R_B); break;
        case (0xf1): CPU::SET(6, R_C); break;
        case (0xf2): CPU::SET(6, R_D); break;
        case (0xf3): CPU::SET(6, R_E); break;
        case (0xf4): CPU::SET(6, R_H); break;
        case (0xf5): CPU::SET(6, R_L); break;
        case (0xf6): 
            CPU::SET(6, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xf7): CPU::SET(6, R_A); break;
        case (0xf8): CPU::SET(7, R_B); break;
        case (0xf9): CPU::SET(7, R_C); break;
        case (0xfa): CPU::SET(7, R_D); break;
        case (0xfb): CPU::SET(7, R_E); break;
        case (0xfc): CPU::SET(7, R_H); break;
        case (0xfd): CPU::SET(7, R_L); break;
        case (0xfe): 
            CPU::SET(7, memory[R.hl]); 
            #ifdef __GHOST_DEBUG
            memory_writes++;
            #endif
            break;
        case (0xff): CPU::SET(7, R_A); break;
    }
}
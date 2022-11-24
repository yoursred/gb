#ifndef __GB_H
#define __GB_H

#include "include.h"
#include "memory/memory.h"


class CPU {
    public:
    // Registers registers;
    // byte memory[0x10000] = {0};
    Memory& memory;

    CPU(Memory& memory);
    byte* mem_at(word* address); byte* mem_at(word address);

    byte opcode = 0;
    byte working_byte = 0;
    word working_word = 0;
    
    bool is_halted = false;
    word new_pc = 0;
    byte current_cycles = 0;
    unsigned long cycles = 0;
    unsigned long instructions = 0;
    

    byte fetch_instruction();
    void step();
    void decode();
    void decode_prefixed();

    class Registers {
        public:
        word *af, *bc, *de, *hl, *sp, *pc;
        byte *a, *f, *b, *c, *d, *e, *h, *l;

        Registers(void);

        void print_regs(void);
        void print_flags(void);
        void set_flag(byte flag);
        void unset_flag(byte flag);
        void flip_flag(byte flag);
        void update_flag(byte flag, word value);
        int get_flag(byte flag);
        int get_cc(byte cc);
        void set_flags(const char *flagstr);
    };

    Registers R;

    private:
    
    // --SECTION-- ARITHMETIC
    void ADC(MP src);
    void ADD(MP src); void ADD(word *src); void ADD_SP(sbyte *src);
    void AND(MP src);
    void CP (MP src);
    void DEC(MP dst); void DEC(word *dst);
    void INC(MP dst); void INC(word *dst);
    void OR (MP src);
    void SBC(MP src);
    void SUB(MP src);
    void XOR(MP src);

    // --SECTION-- BIT OPS
    void BIT(byte bit, MP src);
    void RES(byte bit, MP dst);
    void SET(byte bit, MP dst);
    void SWAP(MP dst);

    // --SECTION-- BIT SHIFTS
    void RL (MP dst);
    void RLA(MP dst);
    void RLC(MP dst);
    void RLCA(MP dst);
    void RR (MP dst);
    void RRA(MP dst);
    void RRC(MP dst);
    void RRCA(MP dst); 
    void SLA(MP dst);
    void SRA(MP dst);
    void SRL(MP dst);

    // --SECTION-- LOAD
    void LD (MP dst, MP src);
    void LD (word *dst, word *src);
    void LD16SP(word dst);
    void LDI(MP dst, MP src);
    void LDD(MP dst, MP src);
    void LDHL(void);
    

    // --SECTION-- JUMPS
    void CALL(word address);
    void CALLC(byte cc, word address);
    void JR (MP offset);
    void JRC(byte cc, MP offset);
    void JP (word address);
    void JPC (byte cc, word address);
    void RET(void);
    void RETC(byte cc);
    void RETI();
    void RST(byte vector);
    
    // --SECTION-- STACK
    void PUSH(word *src);
    void POP(word *dst);

    // --SECTION-- MISC
    void CCF(void);
    void CPL(void);
    void DAA(void);
    void DI (void);
    void EI (void);
    void HALT(void);
    void NOP(void);
    void SCF(void);
    void STOP(void);

};
#endif
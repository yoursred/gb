#ifndef __GB_H
#define __GB_H

#include "include.h"


class Gameboy {
    public:
    // GbRegisters registers;
    byte mem[0x10000] = {0};
    byte* mem_at(word* address); byte* mem_at(word address);

    byte opcode = *mem_at(R.pc);
    byte working_byte = 0;
    word working_word = 0;
    
    word new_pc = 0;
    byte current_cycles = 0;
    unsigned long cycles = 0;
    unsigned long instructions = 0;
    Gameboy();

    byte fetch_instruction();
    void step();
    void decode();
    void decode_prefixed();

    class GbRegisters {
        public:
        word *af, *bc, *de, *hl, *sp, *pc;
        byte *a, *f, *b, *c, *d, *e, *h, *l;

        GbRegisters(void);

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

    GbRegisters R;

    private:
    
    // --SECTION-- ARITHMETIC
    void ADC(byte *src);
    void ADD(byte *src); void ADD(word *src); void ADD_SP(sbyte *src);
    void AND(byte *src);
    void CP (byte *src);
    void DEC(byte *dst); void DEC(word *dst);
    void INC(byte *dst); void INC(word *dst);
    void OR (byte *src);
    void SBC(byte *src);
    void SUB(byte *src);
    void XOR(byte *src);

    // --SECTION-- BIT OPS
    void BIT(byte bit, byte *src);
    void RES(byte bit, byte *dst);
    void SET(byte bit, byte *dst);
    void SWAP(byte *dst);

    // --SECTION-- BIT SHIFTS
    void RL (byte *dst);
    void RLA(byte *dst);
    void RLC(byte *dst);
    void RLCA(byte *dst);
    void RR (byte *dst);
    void RRA(byte *dst);
    void RRC(byte *dst);
    void RRCA(byte *dst); 
    void SLA(byte *dst);
    void SRA(byte *dst);
    void SRL(byte *dst);

    // --SECTION-- LOAD
    void LD (byte *dst, byte *src);
    void LD (word *dst, word *src);
    void LD16SP(word dst);
    void LDI(byte *dst, byte *src);
    void LDD(byte *dst, byte *src);
    void LDHL(void);
    

    // --SECTION-- JUMPS
    void CALL(word *address);
    void CALLC(byte cc, word *address);
    void JR (sbyte *offset);
    void JRC(byte cc, sbyte *offset);
    void JP (word *address);
    void JPC (byte cc, word *address);
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
#ifndef __GB_H
#define __GB_H

#include "include.h"


class Gameboy {
    public:
    // GbRegisters registers;
    byte mem[0x10000];
    byte* mem_at(word *address); byte* mem_at(word address);

    byte opcode;
    byte working_byte;
    word working_word;
    
    word new_pc;
    unsigned long cycles;
    Gameboy();

    byte fetch_instruction();
    void step();

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
        void set_flags(const char *flagstr, const byte *a, const byte *b, const byte result);
    };

    GbRegisters registers;

    private:
    
    // --SECTION-- ARITHMETIC
    void ADC(byte *src);
    void ADD(byte *src); void ADD(word *src); void ADD_SP(byte *src);
    void AND(byte *src);
    void CP (byte *src);
    void DEC(byte *dst); void DEC(word *dst);
    void INC(byte *dst); void INC(word *dst);
    void OR (byte *src);
    void SBC(byte *src);
    void SUB(byte *src);
    void XOR(byte *src);


    // void SUB(Operand<byte> dst);
    // void XOR(Operand<byte> dst);

    // --SECTION-- LOAD
    // template <typename T, typename U> void LD(Operand<T> dst, Operand<U> src);

    // --SECTION-- JUMPS
    void JP (word *offset);
    
};
#endif
#ifndef __GB_H
#define __GB_H

#include "include.h"

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
    int get_flag(byte flag);
    int get_cc(byte cc);
};

class Gameboy {
    public:
    GbRegisters registers;
    byte mem[0x10000];

    byte opcode;
    byte working_byte;
    word working_word;
    
    byte new_pc;
    unsigned long cycles;
    Gameboy();

    void fetch_instruction();

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
        int get_flag(byte flag);
        int get_cc(byte cc);
    };

    private:
    
    // --SECTION-- ARITHMETIC
    void INC(byte *dst);
    void INC(word *dst);
    // void SUB(Operand<byte> dst);
    // void XOR(Operand<byte> dst);

    // --SECTION-- LOAD
    // template <typename T, typename U> void LD(Operand<T> dst, Operand<U> src);
    
};
#endif
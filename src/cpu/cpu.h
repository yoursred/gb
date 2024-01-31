#ifndef __CPU_H
#define __CPU_H

#include "include.h"
#include "memory/memory.h"
#include <string>


#define R_A MP(R.af, R_HI)
#define R_F MP(R.af, R_LO)
#define R_B MP(R.bc, R_HI)
#define R_C MP(R.bc, R_LO)
#define R_D MP(R.de, R_HI)
#define R_E MP(R.de, R_LO)
#define R_H MP(R.hl, R_HI)
#define R_L MP(R.hl, R_LO)

#define EI_0 1
#define EI_1 2
#define DI_0 4
#define DI_1 8

#define INT_VBLANK    1
#define INT_LCD_STAT  2
#define INT_TIMER     4
#define INT_SERIAL    8
#define INT_JOYPAD   16

#define TAC_ENABLE 4
#define TAC_CS 0b11

class CPU {
    public:
    // Registers registers;
    // byte memory[0x10000] = {0};
    Memory& memory;

    CPU(Memory& memory);
    // byte* mem_at(word* address); byte* mem_at(word address);

    byte opcode = 0;
    byte working_byte = 0;
    word working_word = 0;
    
    word new_pc = 0x100;
    byte current_cycles = 0;
    unsigned long cycles = 0;
    unsigned long instructions = 0;

    // Interrupt stuff
    bool is_halted = false;
    bool ime = false;
    byte ime_buffer = 0;
    byte &IF, &IE;
    bool int_trig = false;

    // Timer stuff
    // TODO: Move to separate file/class
    byte &DIV, &TIMA, &TMA, &TAC;
    // word timer_timer = 0; // Yo dawg, I heard you liked timers so I put a timer on your timer
    word div_timer = 0;
    // size_t timer_debug = 0;
    // size_t div_debug = 0;
    bool last_div_bit = false;
    bool tima_reload = false;
    byte tima_reload_pipe = 0;
    

    byte fetch_instruction();
    byte prefetch();
    void step();
    void decode();
    void decode_prefixed();

    bool handle_interrupt();

    void cycle(byte count);
    void timer_tick();

    #ifdef __GHOST_DEBUG
    size_t memory_writes = 0;
    #endif

    class Registers {
        public:
        word af, bc, de, hl, sp, pc;
        // byte *a, *f, *b, *c, *d, *e, *h, *l;

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

    std::string log();

    private:
    
    // --SECTION-- ARITHMETIC
    void ADC(MP src);
    void ADD(MP src); void ADD(word& src); void ADD_SP(sbyte src);
    void AND(MP src);
    void CP (MP src);
    void DEC(MP dst); void DEC(word& dst);
    void INC(MP dst); void INC(word& dst);
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
    void LD (word& dst, word& src);
    void LD16SP(word dst);
    void LDI(MP dst, MP src);
    void LDD(MP dst, MP src);
    void LDHL(void);
    

    // --SECTION-- JUMPS
    void CALL(word address);
    void CALLC(byte cc, word address);
    void JR  (sbyte offset);
    void JRC (byte cc, sbyte offset);
    void JP  (word address);
    void JPHL(void);
    void JPC (byte cc, word address);
    void RET (void);
    void RETC(byte cc);
    void RETI(void);
    void RST (byte vector);
    
    // --SECTION-- STACK
    void PUSH(word& src);
    void POP(word& dst);

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
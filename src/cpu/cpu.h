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

#define OP_ADD    0
#define OP_SUB    1
#define OP_ADC    2
#define OP_SBC    4
#define OP_ARTM   7
#define OP_LGC    8
#define OP_ADD16 16
#define OP_INC   32
#define OP_DEC   64
#define OP_IDU   96

#define EI_0 1
#define EI_1 2
#define DI_0 4
#define DI_1 8

#define IRQ_VBLANK    1
#define IRQ_LCD_STAT  2
#define IRQ_TIMER     4
#define IRQ_SERIAL    8
#define IRQ_JOYPAD   16

#define TAC_ENABLE 4
#define TAC_CS 0x3

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
    byte mcycles, tcycles = 0;
    unsigned long cycles = 0;
    unsigned long instructions = 0;

    // Interrupt stuff
    bool is_halted = false;
    bool ime = false;
    byte ime_buffer = 0;
    byte &IF, &IE;
    bool last_stat_int = false; // This is to detect a rising edge
    bool last_joyp_int = false;
    bool int_trig = false;

    // Timer stuff
    // TODO: Move to separate file/class
    byte &JOY;
    byte &SB, &SC;
    byte &DIV, &TIMA, &TMA, &TAC;
    
    // word timer_timer = 0; // Yo dawg, I heard you liked timers so I put a timer on your timer
    word div_timer = 0;
    // size_t timer_debug = 0;
    // size_t div_debug = 0;
    bool last_div_bit = false;
    bool tima_reload = false;
    byte tima_reload_pipe = 0;


    
    byte fetch();
    byte fetch_instruction();
    byte prefetch();
    void step();
    void decode();
    void decode_prefixed();

    bool handle_interrupt();

    byte machine_cycle();
    void tick();
    void cycle(byte count);
    void timer_tick();

    #ifdef __GHOST_DEBUG
    size_t memory_writes = 0;
    #endif

    class Registers {
        public:
        word af, bc, de, hl, sp, pc, wz;
        byte &a, &f, &b, &c, &d, &e, &h, &l, &spl, &sph, &z, &w;

        Registers();

        void print_regs(std::ostream& output);
        void print_flags(std::ostream& output);
        void set_flag(byte flag);
        void unset_flag(byte flag);
        void flip_flag(byte flag);
        void update_flag(byte flag, word value);
        int get_flag(byte flag);
        int get_cc(byte cc);
        void set_flags(const char *flagstr);
        void set_flags(const char *flagstr, byte operation, byte op1, byte op2 = 1);

    };

    CPU::Registers R;

    std::string log();

    private:
    // --SECTION-- 8-BIT LOADS
    void LD(byte& r8, byte& r8p); void LD(byte& r8);
    void LD(byte& r8, word& r16ptr); void LD(word& r16ptr, byte& r8);
    void LD_A_d16ptr(); void LD_d16ptr_A();
    void LDH_A_Cptr(); void LDH_Cptr_A();
    void LDH_A_d8ptr(); void LDH_d8ptr_A();
    void LD_A_HLdptr(); void LD_HLdptr_A();
    void LD_A_HLiptr(); void LD_HLiptr_A();

    // --SECTION--16-BIT LOADS
    void LD(word& r16);
    void LD_d16ptr_SP();
    void LD_SP_HL();
    void PUSH(word& r16);
    void POP(word& r16);
    void LD_HL_SP();

    // --SECTION-- 8-BIT ARITHMETIC
    void ADD(byte& r8); void ADD_HLptr(); void ADD_d8();
    void ADC(byte& r8); void ADC_HLptr(); void ADC_d8();
    void SUB(byte& r8); void SUB_HLptr(); void SUB_d8();
    void SBC(byte& r8); void SBC_HLptr(); void SBC_d8();
    void CP (byte& r8); void CP_HLptr (); void CP_d8 ();
    void INC(byte& r8); void INC_HLptr(); void INC_d8();
    void DEC(byte& r8); void DEC_HLptr(); void DEC_d8();
    void AND(byte& r8); void AND_HLptr(); void AND_d8();
    void OR (byte& r8); void OR_HLptr (); void OR_d8 ();
    void XOR(byte& r8); void XOR_HLptr(); void XOR_d8();
    void CCF(); void SCF(); void CPL(); void DAA();

    // --SECTION--16-BIT ARITHMETIC
    void INC(word& r16);
    void DEC(word& r16);
    void ADD_HL(word& r16);
    void ADD_SP();

    // void ADD_SP(sbyte src);

    // void ADC(MP src);
    // void ADD(MP src); void ADD(word& src); 
    // void AND(MP src);
    // void CP (MP src);
    // void DEC(MP dst); 
    // void INC(MP dst); 
    // void OR (MP src);
    // void SBC(MP src);
    // void SUB(MP src);
    // void XOR(MP src);

    // --SECTION-- BIT OPS
    void RLCA(); void RRCA();
    void RLA(); void RRA();
    void RLC(byte& r8); void RLC_HLptr();
    void RR(byte& r8); void RR_HLptr();
    void SLA(byte& r8); void SLA_HLptr();
    void SRA(byte& r8); void SRA_HLptr();
    void SWAP(byte& r8); void SWAP_HLptr();
    void SRL(byte& r8); void SRL_HLptr();
    void BIT(byte& r8); void BIT_HLptr();
    void RES(byte& r8); void RES_HLptr();
    void SET(byte& r8); void SET_HLptr();

    // --SECTION-- JUMPS
    void JP(); void JP_HL(); void JP(byte cc);
    void JR(); void JR(byte cc);
    void CALL(); void CALL(byte cc);
    void RET(); void RET(byte cc); void RETI();
    void RST(byte vector);

    // --SECTION-- MISC
    void HALT();
    void STOP();
    void DI();
    void EI();
    void NOP();

    // void BIT(byte bit, MP src);
    // void RES(byte bit, MP dst);
    // void SET(byte bit, MP dst);
    // void SWAP(MP dst);

    // // --SECTION-- BIT SHIFTS
    // void RL (MP dst);
    // void RLA(MP dst);
    // void RLC(MP dst);
    // void RLCA(MP dst);
    // void RR (MP dst);
    // void RRA(MP dst);
    // void RRC(MP dst);
    // void RRCA(MP dst); 
    // void SLA(MP dst);
    // void SRA(MP dst);
    // void SRL(MP dst);

    // // --SECTION-- LOAD
    // void LD (MP dst, MP src);
    // void LD (word& dst, word& src);
    // void LD16SP(word dst);
    // void LDI(MP dst, MP src);
    // void LDD(MP dst, MP src);
    // void LDHL(void);
    

    // // --SECTION-- JUMPS
    // void CALL(word address);
    // void CALLC(byte cc, word address);
    // void JR  (sbyte offset);
    // void JRC (byte cc, sbyte offset);
    // void JP  (word address);
    // void JPHL(void);
    // void JPC (byte cc, word address);
    // void RET (void);
    // void RETC(byte cc);
    // void RETI(void);
    // void RST (byte vector);
    
    // // --SECTION-- STACK
    // void PUSH(word& src);
    // void POP(word& dst);

    // // --SECTION-- MISC
    
    // void DI (void);
    // void EI (void);
    // void HALT(void);
    // void NOP(void);
    // void STOP(void);

};
#endif
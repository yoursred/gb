#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>

#include "cpu/cpu.h"
#include "cpu/insset.h"


CPU::CPU(Memory& memory): 
    memory(memory),
    IF(memory.IO_R[0xF]),
    IE(memory.IE),
    DIV(memory.IO_R[4]),
    TIMA(memory.IO_R[5]),
    TMA(memory.IO_R[6]),
    TAC(memory.IO_R[7])
{
    CPU::R = CPU::Registers();
    instructions = 0;
}

// byte* CPU::mem_at(word* address) {
//     return &memory[*address];
// }

// byte* CPU::mem_at(word address) {
//     return &memory[address];
// }

byte CPU::fetch_instruction() {
    CPU::opcode = memory[R.pc];
    byte length;
    if (opcode != 0xCB){
        length = get_length(CPU::opcode);
        CPU::current_cycles += get_cycles(CPU::opcode);
    } else {
        length = get_length_prefixed(CPU::opcode); // Wholly redundant, it's always =2
        CPU::current_cycles += get_cycles_prefixed(CPU::opcode);
    }
    // printf("opc=0x%02X, length=%d\n", CPU::opcode, length);
    switch (length) { // maybe we don't really need this?
        case 3:
            working_word = memory[R.pc + 1];
            working_word |= (word) memory[R.pc + 2] << 8;
            break;
        case 2:
            working_byte = memory[R.pc + 1];
            break;
    }
    // printf("wb = 0x%02X, ww = 0x%04X\n", working_byte, working_word);
    return length;
}

byte CPU::prefetch() {
    CPU::opcode = memory[R.pc];
    byte length;
    if (opcode != 0xCB){
        length = get_length(CPU::opcode);
    } else {
        length = get_length_prefixed(CPU::opcode); // Wholly redundant, it's always =2
    }
    // printf("opc=0x%02X, length=%d\n", CPU::opcode, length);
    switch (length) { // maybe we don't really need this?
        case 3:
            working_word = memory[R.pc + 1];
            working_word |= (word) memory[R.pc + 2] << 8;
            break;
        case 2:
            working_byte = memory[R.pc + 1];
            break;
    }
    return length;
}

void CPU::step() {
    // TODO: halt
    timer_tick();
    handle_interrupt();
    if (!is_halted) {
        new_pc += fetch_instruction();
        switch (opcode) {
            case (0x00): break;
            case (0xCB): decode_prefixed(); break;
            default: decode();
        }
        switch (ime_buffer) {
            case EI_0:
                ime_buffer = EI_1;
                break;
            case EI_1:
                // std::cout << "ENABLING INTERRUPTS\n";
                ime = true;
                ime_buffer = 0;
                break;
        }
    }
    else {
        current_cycles = 4;
    }
    instructions++;
    cycles += current_cycles;
    // timer_timer += current_cycles;
    for (;current_cycles > 0; current_cycles--) {
        timer_tick();
    }
    R.pc = new_pc;
    // current_cycles = 0;
}

bool CPU::handle_interrupt() {
    bool ret = false;
    if (ime) {
        // std::cout << "CHECKING FOR INTERRUPT\n";
        if (IF & IE) {
            ime = false;
            // new_pc = R.pc;
            current_cycles += (5 * 4);
            ret = true;
            is_halted = false;
            // std::cout << "INTERRUPT\n";
        }
        switch (IF & IE) {
            case INT_VBLANK:
                RST(0x40);
                IF &= ~INT_VBLANK;
                break;
            case INT_LCD_STAT:
                RST(0x48);
                IF &= ~INT_LCD_STAT;
                break;
            case INT_TIMER:
                RST(0x50);
                IF &= ~INT_TIMER;
                break;
            case INT_SERIAL:
                RST(0x58);
                IF &= ~INT_SERIAL;
                break;
            case INT_JOYPAD:
                RST(0x60);
                IF &= ~INT_JOYPAD;
                break;
            
            default:
                break;
        }
        return ret;
    }
    else if (IE & IF) {
        is_halted = false;
    }
    return false;
}

void CPU::timer_tick() {
    // https://pixelbits.16-b.it/GBEDG/timers/#timer-operation
    div_timer += 4;
    DIV = div_timer >> 8;
    if (tima_reload) {
        TIMA = TMA;
        tima_reload = false;
    }

    /*  TODO: Explain
        0 -> 9
        1 -> 3
        2 -> 5
        3 -> 7
    */
    
    byte div_bit = 2 * ((TAC - 1) & TAC_CS) + 3;
    div_bit = div_timer & (1 << div_bit);
    if (last_div_bit && !(div_bit && (TAC & TAC_ENABLE))) {
        if (TIMA == 0xFF) {
            // TODO: implement timer interrupt abortion
            TIMA = 0;
            tima_reload = true;
            // IF |= INT_TIMER;
        } else  {
            TIMA++;
        }
    }
    last_div_bit = div_bit;

}

std::string CPU::log() {
    std::stringstream out;
    out <<  "A:" << COUT_HEX_BYTE(R_A);
    out << " F:" << COUT_HEX_BYTE(R_F);
    out << " B:" << COUT_HEX_BYTE(R_B);
    out << " C:" << COUT_HEX_BYTE(R_C);
    out << " D:" << COUT_HEX_BYTE(R_D);
    out << " E:" << COUT_HEX_BYTE(R_E);
    out << " H:" << COUT_HEX_BYTE(R_H);
    out << " L:" << COUT_HEX_BYTE(R_L);
    out << " SP:" << COUT_HEX_WORD(R.sp);
    out << " PC:" << COUT_HEX_WORD(R.pc);
    out << " PCMEM:";
    out << COUT_HEX_BYTE(memory[R.pc]) << "," << COUT_HEX_BYTE(memory[R.pc + 1]) << ",";
    out << COUT_HEX_BYTE(memory[R.pc + 2]) << "," << COUT_HEX_BYTE(memory[R.pc + 3]) << std::endl;

    return out.str();
}   
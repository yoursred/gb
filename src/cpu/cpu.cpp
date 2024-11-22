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
    JOY(memory.IO_R[0]),
    SB(memory.IO_R[1]),
    SC(memory.IO_R[2]),
    DIV(memory.IO_R[4]),
    TIMA(memory.IO_R[5]),
    TMA(memory.IO_R[6]),
    TAC(memory.IO_R[7])
{
    CPU::R = CPU::Registers();
    instructions = 0;

    IF = 0xE1;
    IE = 0x00;
    JOY = 0xFF;
    SB = 0x00;
    SC = 0x7E;
    DIV = 0x00;
    TIMA = 0x00;
    TMA = 0x00;
    TAC = 0xF8;
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
        CPU::current_cycles = get_cycles(CPU::opcode);
    } else {
        length = get_length_prefixed(CPU::opcode); // Wholly redundant, it's always =2
        CPU::current_cycles = get_cycles_prefixed(CPU::opcode);
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
    // This is for the deboogger
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

byte CPU::fetch() {
    CPU::opcode = memory[R.pc];
    R.pc++;
}

void CPU::tick() {
    timer_tick();
    if (!is_halted) {
        if ((cc % 4) == 0) {
            // Fire M-cycle operation
            decode();
            if (cc == 4) {
                fetch();
            }
        }
        //  else if (cc % 4 == 0) {
        //     if (cc == 4)
        //         fetch_instruction();
        // } 
        // else {
            // else if (cc % 4 == 0); // Queue next M-cycle operation
            // Note: This shouldn't be needed in theory, the instruction methods (`CPU::INC(byte dst)`
            //       for example) should be able to handle their own actions. To what extent they'll
            //       need to be modified still needs to be determined. `CPU::LD(byte dst, byte src)`
            //       has the same timing so long as both `src` and `dst` are 8-bit registers. 
            //       Only methods with more than a single M-cycle (less than half) will need this new 
            //       timing quirk.
            
        // }
    }
    cc--;
}

void CPU::step() {
    // TODO: halt
    // timer_tick();
    if (!is_halted) {
        // if (!int_trig){
            new_pc += fetch_instruction();
        // } else {
        //     fetch_instruction();
        // }

        for (byte i = 0; i < current_cycles; i++) {
            timer_tick();
        }

        switch (opcode) {
            case (0x00): break;
            // case (0xB6):
            //     timer_tick();
            //     timer_tick();
            //     timer_tick();
            //     timer_tick();
            //     timer_tick();
            //     timer_tick();
            //     decode();
            //     break;
            case (0xCB): decode_prefixed(); break;
            default: decode();
        }
        // } else {
            
        // }
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
        for (byte i = 0; i < current_cycles; i++) {
            timer_tick();
        }
    }
    instructions++;
    cycles += current_cycles;
    // timer_timer += current_cycles;
    // if (opcode != 0xB6 || true) { // Memory timing test
    //     for (byte i = 0; i < current_cycles; i++) {
    //         timer_tick();
    //     }
    // } else {
    //     // std::cout << "Encountered F0" << std::endl;
    // }
    R.pc = new_pc;
    // current_cycles = 0;
    // last_stat_int = IF & INT_LCD_STAT;
    int_trig = handle_interrupt();
    // timer_tick();
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
            // std::cout << COUT_HEX_BYTE_DS(IF) << " " << COUT_HEX_BYTE_DS(IE)
        }
        switch (IF & IE) {
            case INT_VBLANK:
                std::cout << std::endl << "VBLANK IRQ" << std::endl;
                std::cout << "> ";
                RST(0x40);
                IF &= ~INT_VBLANK;
                break;
            case INT_LCD_STAT:
                // if (!last_stat_int) { // rising-edge detection
                std::cout << std::endl << "STAT IRQ" << std::endl;
                std::cout << "> ";
                RST(0x48);
                IF &= ~INT_LCD_STAT;
                // }
                break;
            case INT_TIMER:
                // std::cout << "INT_TIMER" << std::endl;
                // std::cout << log();
                memory[--(R.sp)] = R.pc >> 8;
                memory[--(R.sp)] = R.pc;
                R.pc = 0x50;
                new_pc = 0x50;
                IF &= ~INT_TIMER;
                // std::cout << log();
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
        switch (IE & IF) {
            case INT_LCD_STAT:
            break;
            case INT_JOYPAD:
            break;
            default:
            is_halted = false;
            break;
        }
    }
    return false;
}

void CPU::cycle(byte count) {
    for (;count > 0; count--) {

    }
}

void CPU::timer_tick() {
    // -https://pixelbits.16-b.it/GBEDG/timers/#timer-operation- DEAD LINK
    // https://hacktix.github.io/GBEDG/timers/#timer-operation
    div_timer += 4;
    DIV = div_timer >> 8;
    if (tima_reload) {
        if (tima_reload_pipe == 4) {
            TIMA = TMA;
            tima_reload = false;
            tima_reload = 0;
        } else {

        }
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
            // UPDATE: See Roe v. Wade
            TIMA = 0;
            tima_reload = true;
            tima_reload_pipe = 1;
            IF |= INT_TIMER; // Why was this commented out?
        } else  {
            TIMA++;
        }
    }
    last_div_bit = div_bit;
    tima_reload_pipe++;
}

std::string CPU::log() {
    std::stringstream out;
    out <<  "A: " << COUT_HEX_BYTE(R_A);
    out << " F: " << COUT_HEX_BYTE(R_F);
    out << " B: " << COUT_HEX_BYTE(R_B);
    out << " C: " << COUT_HEX_BYTE(R_C);
    out << " D: " << COUT_HEX_BYTE(R_D);
    out << " E: " << COUT_HEX_BYTE(R_E);
    out << " H: " << COUT_HEX_BYTE(R_H);
    out << " L: " << COUT_HEX_BYTE(R_L);
    out << " SP: " << COUT_HEX_WORD(R.sp);
    out << " PC: 00:" << COUT_HEX_WORD(R.pc);
    // out << " PCMEM:";
    out << " (";
    out << COUT_HEX_BYTE(memory[R.pc]) << " " << COUT_HEX_BYTE(memory[R.pc + 1]) << " ";
    out << COUT_HEX_BYTE(memory[R.pc + 2]) << " " << COUT_HEX_BYTE(memory[R.pc + 3]) << ")" << std::endl;

    return out.str();
}   
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>

#include "cpu/cpu.h"
#include "cpu/insset.h"


CPU::CPU(Memory& memory): 
    // R(Registers()),
    memory(memory),
    IF(memory.IO_R[0xF]),
    IE(memory.IE),
    JOY(memory.IO_R[0]),
    SB(memory.IO_R[1]),
    SC(memory.IO_R[2]),
    DIV(*((byte*) &div_timer + 1)),
    TIMA(memory.IO_R[5]),
    TMA(memory.IO_R[6]),
    TAC(memory.IO_R[7])
{
    // CPU::R( = CPU::Registers());
    // CPU::R = Registers();
    // R();
    instructions = 0;

    IF = 0x00;
    IE = 0x00;
    JOY = 0xFF;
    SB = 0x00;
    SC = 0x7E;
    DIV = 0x00;
    TIMA = 0x00;
    TMA = 0x00;
    TAC = 0xF8;
    doctor_log << log_dr();
    opcode = memory.read(R.pc++);
    current_tcycles = get_cycles(opcode);


    // fetch();
}

// byte* CPU::mem_at(word* address) {
//     return &memory.read(*address);
// }

// byte* CPU::mem_at(word address) {
//     return &memory.read(address);
// }

byte CPU::fetch_instruction() {
    CPU::opcode = memory.read(R.pc);
    byte length;
    if (opcode != 0xCB){
        length = get_length(CPU::opcode);
        CPU::current_tcycles = get_cycles(CPU::opcode);
    } else {
        length = get_length_prefixed(CPU::opcode); // Wholly redundant, it's always =2
        CPU::current_tcycles = get_cycles_prefixed(CPU::opcode);
    }
    // printf("opc=0x%02X, length=%d\n", CPU::opcode, length);
    switch (length) { // maybe we don't really need this?
        case 3:
            working_word = memory.read(R.pc + 1);
            working_word |= (word) memory.read(R.pc + 2) << 8;
            break;
        case 2:
            working_byte = memory.read(R.pc + 1);
            break;
    }
    // printf("wb = 0x%02X, ww = 0x%04X\n", working_byte, working_word);
    return length;
}

byte CPU::prefetch() {
    // This is for the deboogger
    CPU::opcode = memory.read(R.pc);
    byte length;
    if (opcode != 0xCB){
        length = get_length(CPU::opcode);
    } else {
        length = get_length_prefixed(CPU::opcode); // Wholly redundant, it's always =2
    }
    // printf("opc=0x%02X, length=%d\n", CPU::opcode, length);
    switch (length) { // maybe we don't really need this?
        case 3:
            working_word = memory.read(R.pc + 1);
            working_word |= (word) memory.read(R.pc + 2) << 8;
            break;
        case 2:
            working_byte = memory.read(R.pc + 1);
            break;
    }
    return length;
}

byte CPU::fetch() {
    if (log_lines > 0 && (prefixed_fetch != 1)) {
        doctor_log << log_dr();
        if (!(--log_lines))
            std::cout << std::endl << "DONE" << std::endl << "> ";
    } else {
        // std::cout << "DONE" << std::endl;
    }

    switch (ime_buffer) {
        case EI_0:
            ime_buffer = EI_1;
            break;
        case EI_1:
            // std::cout << "ENABLING INTERRUPTS\n";
            std::cout << "IRQ SERVICING ON" << std::endl;
            ime = true;
            ime_buffer = 0;
            break;
    }
    
    if ((IF & IE) && ime) {
        std::cout << "INTERRUPT " << COUT_HEX_BYTE(IF) << std::endl; 
    }

    interrupt = 0;
    opcode = memory.read(R.pc++);
    tcycles = 0;
    current_tcycles = get_cycles(opcode);
    if (prefixed_fetch == 1) {
        prefixed_fetch++;
        current_tcycles = get_cycles_prefixed(opcode);
    } else {
        prefixed_fetch %= 2;
        // if (opcode != 0xF3)
            if (handle_interrupt()) {
                current_tcycles = 20;
            }
    }

    // tcycles = get_cycles(opcode);
    // mcycles = tcycles / 4;
    // R.pc++;
    return 0;
}

void CPU::tick() {
    if (!halted) {
        if ((tcycles % 4) == 0) {
            timer_tick();
            // Fire M-cycle operation
            if (prefixed_fetch)
                decode_prefixed();
            else if (interrupt)
                INT(interrupt);
            else
                decode();
        }
        if ((current_tcycles - (++tcycles)) == 0)
            fetch();
}
}

void CPU::step() {
    // TODO: halt
    // timer_tick();
    if (!halted) {
        // if (!int_trig){
            new_pc += fetch_instruction();
        // } else {
        //     fetch_instruction();
        // }

        for (byte i = 0; i < current_tcycles; i++) {
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
            // case (0xCB): decode_prefixed(); break;
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
        current_tcycles = 4;
        for (byte i = 0; i < current_tcycles; i++) {
            timer_tick();
        }
    }
    instructions++;
    cycles += current_tcycles;
    // timer_timer += current_tcycles;
    // if (opcode != 0xB6 || true) { // Memory timing test
    //     for (byte i = 0; i < current_tcycles; i++) {
    //         timer_tick();
    //     }
    // } else {
    //     // std::cout << "Encountered F0" << std::endl;
    // }
    R.pc = new_pc;
    // current_tcycles = 0;
    // last_stat_int = IF & INT_LCD_STAT;
    interrupt = handle_interrupt();
    // timer_tick();
}

bool CPU::handle_interrupt() {
    // if (opcode == 0xF3) {
    //     return false;
    // }
    bool ret = false;
    if (ime) {
        // std::cout << "CHECKING FOR INTERRUPT\n";
        if (IF & IE) {
            ime = false;
            // new_pc = R.pc;
            // current_tcycles += (5 * 4);
            ret = true;
            halted = false;
            // std::cout << "INTERRUPT\n";
            // std::cout << COUT_HEX_BYTE_DS(IF) << " " << COUT_HEX_BYTE_DS(IE)
        }
        if (IF & IRQ_VBLANK) {
            IF &= ~IRQ_VBLANK;
            interrupt = 0x40;
        } else if (IF & IE & IRQ_LCD_STAT) {
            IF &= ~IRQ_LCD_STAT;
            interrupt = 0x48;
        } else if (IF & IE & IRQ_TIMER) {
            interrupt = 0x50;
            IF &= ~IRQ_TIMER;
        } else if (IF & IE & IRQ_SERIAL) {
            IF &= ~IRQ_SERIAL;
            interrupt = 0x58;
        } else if (IF & IE & IRQ_JOYPAD) {
            interrupt = 0x60;
            IF &= ~IRQ_JOYPAD;
        }
            // case IRQ_VBLANK:
            //     // std::cout << std::endl << "VBLANK IRQ" << std::endl;
            //     // std::cout << "> ";
            //     // vector
            //     IF &= ~IRQ_VBLANK;
            //     interrupt = 0x40;
            //     break;
            // case IRQ_LCD_STAT:
            //     // if (!last_stat_int) { // rising-edge detection
            //     // std::cout << std::endl << "STAT IRQ" << std::endl;
            //     // std::cout << "> ";
            //     // RST(0x48);
            //     IF &= ~IRQ_LCD_STAT;
            //     interrupt = 0x48;
            //     // }
            //     break;
            // case IRQ_TIMER:
            //     // std::cout << "IRQ_TIMER" << std::endl;
            //     // std::cout << log();
            //     // memory.write(--(R.sp), R.pc >> 8);
            //     // memory.write(--(R.sp), R.pc);
            //     // R.pc = 0x50;
            //     // new_pc = 0x50;
            //     interrupt = 0x50;
            //     IF &= ~IRQ_TIMER;
            //     // std::cout << log();
            //     break;
            // case IRQ_SERIAL:
            //     // RST(0x58);
            //     IF &= ~IRQ_SERIAL;
            //     interrupt = 0x58;
            //     break;
            // case IRQ_JOYPAD:
            //     // RST(0x60);
            //     interrupt = 0x60;
            //     IF &= ~IRQ_JOYPAD;
            //     break;
            // default:
            //     std::cout << "HCF CONDITION" << std::endl;
            //     break;
        
        return ret;
    }
    // else if ((IE & IF)) {
    //     switch (IE & IF) {
    //         case IRQ_VBLANK:
    //         IF &= ~IRQ_VBLANK;
    //         break;
    //         case IRQ_LCD_STAT:
    //         IF &= ~IRQ_LCD_STAT;
    //         break;
    //         case IRQ_JOYPAD:
    //         IF &= ~IRQ_JOYPAD;
    //         break;
    //         case IRQ_TIMER:
    //         IF &= ~IRQ_TIMER;
    //         break;
    //         default:
    //         break;
    //         halted = false;
    //     }
    // }
    return false;
}

void CPU::timer_tick() {
    // -https://pixelbits.16-b.it/GBEDG/timers/#timer-operation- DEAD LINK
    // https://hacktix.github.io/GBEDG/timers/#timer-operation
    div_timer++;
    // div_timer += 4;
    if (tima_reload) {
        if (tima_reload_pipe == 3) {
            TIMA = TMA;
            tima_reload = false;
            tima_reload_pipe = 0;
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
    div_bit = !!(div_timer & (1 << div_bit));
    if (last_div_bit && !(div_bit && (TAC & TAC_ENABLE))) {
        if (TIMA == 0xFF) {
            // TODO: implement timer interrupt abortion
            // UPDATE: See Roe v. Wade
            TIMA = 0;
            tima_reload = true;
            tima_reload_pipe = 0;
            // IF |= IRQ_TIMER; // Why was this commented out?
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
    out << COUT_HEX_BYTE(memory.read(R.pc)) << " " << COUT_HEX_BYTE(memory.read(R.pc + 1)) << " ";
    out << COUT_HEX_BYTE(memory.read(R.pc + 2)) << " " << COUT_HEX_BYTE(memory.read(R.pc + 3)) << ")" << std::endl;

    return out.str();
}

std::string CPU::log_dr() {
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
    out << COUT_HEX_BYTE(memory.read(R.pc)) << "," << COUT_HEX_BYTE(memory.read(R.pc + 1)) << ",";
    out << COUT_HEX_BYTE(memory.read(R.pc + 2)) << "," << COUT_HEX_BYTE(memory.read(R.pc + 3)) << std::endl;

    return out.str();
}

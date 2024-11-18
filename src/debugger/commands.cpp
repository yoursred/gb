#include <iostream>
#include <stdexcept>

#include "include.h"
#include "debugger/debug.h"



void Debugger::cmd_break(std::vector<std::string> argv) {
    word x;
    byte y;
    switch (argv.size()) {
        case 0:
        output << "Argument error: not enough arguments!" << std::endl;
        break;
        case 1: // A PC breakpoint
        if (hex_word(argv[0], x)) {
            Breakpoint b(pc, x);
            output << b.str() << std::endl;
            breakpoints.push_back(b);
        } else {
            output << "Argument error: `" << argv[0];
            output << "` not a valid value for watch type `PC`." << std::endl;
        }
        break;
        case 2:
        // if (str())
        if (!str(argv[1], Debugger::break_types)) {
            output << "Argument error: `" << argv[1];
            output << "` not a valid register name!" << std::endl;
            break;
        } else if (argv[1] == "wmem") {
            output << "Argument error: not enough arguments for watch type `" << argv[1];
            output << "`." << std::endl;
        } else if (str(argv[1], Debugger::break16_types) && hex_word(argv[0], x)) {
            Breakpoint b(argv[0], x, 0);
            output << b.str() << std::endl;
            breakpoints.push_back(b);
        } else if (str(argv[1], Debugger::reg8_types) && hex_byte(argv[0], y)) {
            Breakpoint b(argv[0], 0, y);
            output << b.str() << std::endl;
            breakpoints.push_back(b);
        } else {
            output << "Argument error: `" << argv[0];
            output << "` not a valid value for watch type `";
            output << argv[1] << "`." << std::endl;
        }
        break;
        case 3:
        if (argv[1] != "wmem") {
            output << "Argument error: too many arguments for watch type `" << argv[1];
            output << "`." << std::endl;
        } else if (!hex_word(argv[0], x)) {
            output << "Argument error: `" << argv[0];
            output << "` not a valid memory address.";
        } else if (!hex_byte(argv[2], y)) {
            output << "Argument error: `" << argv[2];
            output << "` not a valid memory value.";
        } else {
            Breakpoint b(argv[1], x, y);
            output << b.str() << std::endl;
            breakpoints.push_back(b);
        }
        break;
        default:
        output << "Syntax error." << std::endl;
        break;
    }
}

void Debugger::cmd_clear() {
    breakpoints.clear();
}

void Debugger::cmd_continue() {
    state = DBG_RUNNING;
}

void Debugger::cmd_cycles() {
    output << std::dec << cpu.cycles << std::endl;
}

void Debugger::cmd_delete(std::vector<std::string> argv) {
    unsigned int x;
    if (argv.size() != 1) {
        output << "Syntax error." << std::endl;
        return;
    }

    if (!dec_uint(argv[0], x)) {
        output << "Argument error: index `" << argv[0];
        output << "` is not valid." << std::endl;
    } else if (!(x < breakpoints.size())) {
        output << "Value error: index `" << argv[0];
        output << "` is out of range." << std::endl;
    } else {
        breakpoints.erase(breakpoints.begin() + x);
        output << "Deleted breakpoint " << x << "." << std::endl;
    }
}

void Debugger::cmd_dump(std::vector<std::string> argv) {
    word x, y;
    switch (argv.size()) {
        case 0:
        dumphex(mem, cpu.R.pc, 0x10, output);
        break;
        case 1:
        if (hex_word(argv[0], x)) {
            dumphex(mem, x, 0x10, output);
        } else {
            output << "Argument error: index `" << argv[0];
            output << "` is not valid." << std::endl;
        }
        break;
        case 2:
        if (!hex_word(argv[0], x)) {
            output << "Argument error: index `" << argv[0];
            output << "` is not valid." << std::endl;
        } else if (!hex_word(argv[1], y)) {
            output << "Argument error: index `" << argv[1];
            output << "` is not valid." << std::endl;
        } else {
            dumphex(mem, x, y, output);
        }
        break;
        default:
        output << "Syntax error: too many arguments." << std::endl;
        break;
    }
}

void Debugger::cmd_exit() {
    state = DBG_END;
}

void Debugger::cmd_flags() {
    cpu.R.print_flags(output);
}

void Debugger::cmd_ic() {
    output << cpu.instructions << std::endl;
}

void Debugger::cmd_list() {
    unsigned int b_count, p_width;
    for (unsigned int i = 0; i < breakpoints.size(); i++) {
        b_count = breakpoints.size();
        p_width = 0;
        for (;b_count > 0; b_count /= 10) p_width++;
        output << std::string(p_width, ' ') <<  i << " - "  << breakpoints[i].str() << std::endl;
    }
}

void Debugger::cmd_opc() {
    output << print_instruction(cpu) << std::endl;
}

void Debugger::cmd_pause() {
    state = DBG_PAUSED;
}

void Debugger::cmd_ppu() {
    ppu.print_ppu_registers(output);
}

void Debugger::cmd_registers() {
    cpu.R.print_regs(output);
}

void Debugger::cmd_state() {
    switch (state) {
        case DBG_STOPPED: output << "DBG_STOPPED" << std::endl; break;
        case DBG_RUNNING: output << "DBG_RUNNING" << std::endl; break;
        case DBG_PAUSED: output << "DBG_PAUSED" << std::endl; break;
        case DBG_END: output << "DBG_END" << std::endl; break;
    }
}

void Debugger::cmd_step() {
    if (state == DBG_PAUSED) {
        cpu.step();
        for (i = 0; i < cpu.current_cycles; i++) {
            ppu.tick();
        }
        print_instruction(cpu);
    }
}